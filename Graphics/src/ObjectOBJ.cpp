#include "ObjectOBJ.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include <stb_image.h>

ObjectOBJ::ObjectOBJ(unsigned int _programID)
{
	Init(_programID);
}

ObjectOBJ::ObjectOBJ(const char* _path, unsigned int _programID)
{
	Init(_programID);
	LoadFromObj(_path);
}

void ObjectOBJ::Init(unsigned int _programID)
{
	m_indexCount = 0;
	m_vertexCount = 0;
	m_programID = _programID;
	m_MVP = glm::mat4(1);
}

ObjectOBJ::~ObjectOBJ()
{

}

void ObjectOBJ::Update(double dt)
{

}

void ObjectOBJ::Draw(Camera* _camera)
{
	glUseProgram(m_programID);
	glEnable(GL_DEPTH_TEST);

	unsigned int projectionUniform = glGetUniformLocation(m_programID, "projection");
	unsigned int modelUniform = glGetUniformLocation(m_programID, "modelView");
	unsigned int diffuse = glGetUniformLocation(m_programID, "diffuse");
	unsigned int ambientColorUniform = glGetUniformLocation(m_programID, "ambientColor");
	unsigned int diffuseColorUniform = glGetUniformLocation(m_programID, "diffuseColor");
	unsigned int specColorUniform = glGetUniformLocation(m_programID, "specColor");
	unsigned int lightDirUniform = glGetUniformLocation(m_programID, "LightDir");
	unsigned int cameraPosUniform = glGetUniformLocation(m_programID, "CameraPos");
	
	glUniformMatrix4fv(projectionUniform, 1, false, glm::value_ptr(_camera->GetProjectionView()));
	glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(m_MVP));
	glUniform1i(diffuse, 3);
	glUniform3fv(ambientColorUniform, 1, glm::value_ptr(ambientColor));
	glUniform3fv(diffuseColorUniform, 1, glm::value_ptr(diffuseColor));
	glUniform3fv(specColorUniform, 1, glm::value_ptr(specColor));
	glUniform3fv(lightDirUniform, 1, glm::value_ptr(glm::normalize(glm::vec3(sinf(glfwGetTime() * 0.1f), m_lightHeight, cosf(glfwGetTime() * 0.1f) + 1))));
	glUniform3fv(cameraPosUniform, 1, glm::value_ptr(_camera->GetPosition()));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void ObjectOBJ::LoadFromObj(const char* _path)
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) + (sizeof(glm::vec3))));

	std::vector<Vertex> aoVerticies;
	std::vector<unsigned int> auiIndicies;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<int> seen;

	std::string mtlPath;

	std::ifstream in(_path, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << _path << std::endl; }
	bool Normals = false;
	std::string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));
			Vertex v;
			s >> v.position.x;
			s >> v.position.y;
			s >> v.position.z;
			v.position.w = 1.0f;

			aoVerticies.push_back(v);
			m_vertexCount++;
		}
		else if (line.substr(0, 2) == "f ")
		{
			char dummyChar;
			std::istringstream s(line.substr(2));
			GLushort a, b, c, d;
			GLushort aTex, bTex, cTex, dTex;
			if (Normals) {
				GLushort aNorm, bNorm, cNorm, dNorm;

				s >> a; s >> dummyChar; s >> aTex; s >> dummyChar; s >> aNorm;
				s >> b; s >> dummyChar; s >> bTex; s >> dummyChar; s >> bNorm;
				s >> c; s >> dummyChar; s >> cTex; s >> dummyChar; s >> cNorm;
				s >> d; s >> dummyChar; s >> dTex; s >> dummyChar; s >> dNorm;
				a--; b--; c--; d--;
				aTex--; bTex--; cTex--; dTex--;
				aNorm--; bNorm--; cNorm--; dNorm--;
				
				aoVerticies[a].normal = normals[aNorm];
				aoVerticies[b].normal = normals[bNorm];
				aoVerticies[c].normal = normals[cNorm];
				aoVerticies[d].normal = normals[dNorm];

			} else {

				s >> a; s >> dummyChar; s >> aTex;
				s >> b; s >> dummyChar; s >> bTex;
				s >> c; s >> dummyChar; s >> cTex;
				s >> d; s >> dummyChar; s >> dTex;
				a--; b--; c--; d--;
				aTex--; bTex--; cTex--; dTex--;

			}
			auiIndicies.push_back(a); auiIndicies.push_back(b); auiIndicies.push_back(c);
			auiIndicies.push_back(a); auiIndicies.push_back(c); auiIndicies.push_back(d);

			aoVerticies[a].texCoord = texCoords[aTex];
			aoVerticies[b].texCoord = texCoords[bTex];
			aoVerticies[c].texCoord = texCoords[cTex];
			aoVerticies[d].texCoord = texCoords[dTex];
			m_indexCount += 6;

		}
		else if (line.substr(0, 3) == "vn ")
		{
			Normals = true;
			std::istringstream s(line.substr(3));
			glm::vec3 norm;
			s >> norm.x; s >> norm.y;
			normals.push_back(norm);
		}
		else if (line.substr(0, 3) == "vt ")
		{
			std::istringstream s(line.substr(3));
			glm::vec2 coords;
			s >> coords.x; s >> coords.y;
			coords.y *= -1.0f;
			texCoords.push_back(coords);
		}
		else if (line.substr(0, 7) == "mtllib ")
		{
			std::istringstream s(line.substr(7));
			s >> mtlPath;
			mtlPath = "res/models/" + mtlPath;
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}
	seen.resize(m_vertexCount, 0);
	if (!Normals) {
		for (unsigned int i = 0; i < m_indexCount; i += 3) {
			GLushort ia = auiIndicies[i];
			GLushort ib = auiIndicies[i+1];
			GLushort ic = auiIndicies[i+2];
			glm::vec3 norm = -glm::normalize(glm::cross(glm::vec3(aoVerticies[ic].position) - glm::vec3(aoVerticies[ia].position),
											glm::vec3(aoVerticies[ib].position) - glm::vec3(aoVerticies[ia].position)));
			int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;
			for (int j = 0; j < 3; j++) {
				GLushort cur_v = v[j];
				seen[cur_v]++;
				if (seen[cur_v] == 1) {
					aoVerticies[cur_v].normal = norm;
				}
				else {
					// average
					aoVerticies[cur_v].normal.x = aoVerticies[cur_v].normal.x * (1.0f - 1.0f / seen[cur_v]) + norm.x * 1.0f / seen[cur_v];
					aoVerticies[cur_v].normal.y = aoVerticies[cur_v].normal.y * (1.0f - 1.0f / seen[cur_v]) + norm.y * 1.0f / seen[cur_v];
					aoVerticies[cur_v].normal.z = aoVerticies[cur_v].normal.z * (1.0f - 1.0f / seen[cur_v]) + norm.z * 1.0f / seen[cur_v];
					aoVerticies[cur_v].normal = glm::normalize(aoVerticies[cur_v].normal);
				}
			}

		}
	}

	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Vertex), &aoVerticies[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), &auiIndicies[0], GL_STATIC_DRAW);
	LoadTexture(mtlPath.c_str());
}

void ObjectOBJ::LoadTexture(const char* _path)
{
	std::ifstream in(_path, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << _path << std::endl; }

	std::string line;
	GLfloat x, y, z;
	std::string path;
	while (getline(in, line))
	{
		if (line.substr(0, 3) == "Kd ")
		{
			std::istringstream s(line.substr(3));
			s >> x; s >> y; s >> z;
			diffuseColor = glm::vec3(x, y, z);
		}
		else if (line.substr(0, 3) == "Ka ")
		{
			std::istringstream s(line.substr(3));
			s >> x; s >> y; s >> z;
			ambientColor = glm::vec3(x, y, z);
		}
		else if (line.substr(0, 3) == "Ks ")
		{
			std::istringstream s(line.substr(3));
			s >> x; s >> y; s >> z;
			specColor = glm::vec3(x, y, z);
		}
		else if (line.substr(0, 7) == "map_Kd ")
		{
			std::istringstream s(line.substr(7));
			s >> path;
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}

	if (path.length() > 1)
	{
		std::string data = "res/models/" + path;
		int textureWidth, textureHeight, textureFormat;
		unsigned char* imageData = stbi_load(data.c_str(), &textureWidth, &textureHeight, &textureFormat, STBI_default);


		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		stbi_image_free(imageData);
	}
}

void ObjectOBJ::Translate(glm::vec3 _translation)
{
	m_MVP *= glm::translate(_translation);
}

void ObjectOBJ::SetScale(glm::vec3 _scale)
{
	m_MVP *= glm::scale(_scale);
	m_scale = _scale;
}

glm::vec3 ObjectOBJ::GetPosition()
{
	return m_MVP[3].xyz();
}
