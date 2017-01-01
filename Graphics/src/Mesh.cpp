#include "Mesh.h"
#include "Material.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "Camera.h"
#include <sstream>
#include <stdio.h>
#include "Debug.h"
#include "aieutilities\Gizmos.h"
Mesh::Mesh()
{
	m_vertexCount = 0;
	m_indexCount = 0;
}

Mesh::~Mesh()
{
}

void Mesh::Update(double _dt)
{

}

void Mesh::Draw(Camera* _camera, mat4 _MVP, int _texture)
{
	glUseProgram(m_programID);
	glEnable(GL_DEPTH_TEST);

	unsigned int projectionUniform = glGetUniformLocation(m_programID, "projection");
	unsigned int modelUniform = glGetUniformLocation(m_programID, "modelView");
	unsigned int diffuse = glGetUniformLocation(m_programID, "diffuse");
	//unsigned int ambientColorUniform = glGetUniformLocation(m_programID, "ambientColor");
	//unsigned int diffuseColorUniform = glGetUniformLocation(m_programID, "diffuseColor");
	//unsigned int specColorUniform = glGetUniformLocation(m_programID, "specColor");
	unsigned int lightDirUniform = glGetUniformLocation(m_programID, "LightDir");
	//unsigned int cameraPosUniform = glGetUniformLocation(m_programID, "CameraPos");

	glUniformMatrix4fv(projectionUniform, 1, false, value_ptr(_camera->GetProjectionView()));
	glUniformMatrix4fv(modelUniform, 1, false, value_ptr(_MVP));
	glUniform1i(diffuse, 3);
	//glUniform3fv(ambientColorUniform, 1, value_ptr(vec4(1,1,1,1)));
	//glUniform3fv(diffuseColorUniform, 1, value_ptr(vec4(1, 1, 1, 1)));
	//glUniform3fv(specColorUniform, 1, value_ptr(vec4(1, 1, 1, 1)));
	glUniform3fv(lightDirUniform, 1, value_ptr(normalize(vec3(sinf(glfwGetTime() * 1.0f), 1, cosf(glfwGetTime() * 1.0f) + 1))));
	Gizmos::addSphere(vec3(sinf(glfwGetTime() * 1.0f), 1, cosf(glfwGetTime() * 1.0f) + 1), 1.0f, 10, 10, vec4(1, 1, 0, 1));
	//glUniform3fv(cameraPosUniform, 1, value_ptr(_camera->GetPosition()));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::BeginImport(int _programID)
{
	m_programID = _programID;

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) + (sizeof(vec3))));
	m_importActive = true;
}

void Mesh::FinishImport()
{
	if (!m_importActive)
	{
		Debug::LogError("Error: import hadn't started before Finish was called");
		return;
	}

	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Vertex), &m_verticies[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), &m_indicies[0], GL_STATIC_DRAW);
	m_importActive = false;
}

void Mesh::AddVert(string _line)
{
	istringstream s(_line.substr(2));
	Vertex v;
	s >> v.position.x;
	s >> v.position.y;
	s >> v.position.z;
	v.position.w = 1.0f;

	m_verticies.push_back(v);
	m_vertexCount++;
}
void Mesh::AddFace(string _line, vector<vec2> _texCoords)
{
	char dummyChar;
	istringstream s(_line.substr(2));
	GLushort a, b, c;
	GLushort aTex, bTex, cTex;
	s >> a; s >> dummyChar; s >> aTex;
	s >> b; s >> dummyChar; s >> bTex;
	s >> c; s >> dummyChar; s >> cTex;
	a--; b--; c--;
	aTex--; bTex--; cTex--;

	m_indicies.push_back(a);
	m_indicies.push_back(b); 
	m_indicies.push_back(c);

	m_verticies[a].texCoord = _texCoords[aTex];
	m_verticies[b].texCoord = _texCoords[bTex];
	m_verticies[c].texCoord = _texCoords[cTex];
	m_indexCount += 3;
}
void Mesh::AddFaceWithNormals(string _line, vector<vec2> _texCoords, vector<vec3> _normals)
{
	char dummyChar;
	istringstream s(_line.substr(2));
	GLushort a, b, c;
	GLushort aTex, bTex, cTex;
	GLushort aNorm, bNorm, cNorm;

	s >> a; s >> dummyChar; s >> aTex; s >> dummyChar; s >> aNorm;
	s >> b; s >> dummyChar; s >> bTex; s >> dummyChar; s >> bNorm;
	s >> c; s >> dummyChar; s >> cTex; s >> dummyChar; s >> cNorm;
	a--; b--; c--;
	aTex--; bTex--; cTex--;
	aNorm--; bNorm--; cNorm--;

	m_verticies[a].normal = _normals[aNorm];
	m_verticies[b].normal = _normals[bNorm];
	m_verticies[c].normal = _normals[cNorm];
	m_indicies.push_back(a);
	m_indicies.push_back(b);
	m_indicies.push_back(c);

	m_verticies[a].texCoord = _texCoords[aTex];
	m_verticies[b].texCoord = _texCoords[bTex];
	m_verticies[c].texCoord = _texCoords[cTex];
	m_indexCount += 3;
}

void Mesh::GenerateNormals()
{
	std::vector<int> seen;
	seen.resize(m_vertexCount, 0);
	for (unsigned int i = 0; i < m_indexCount; i += 3) {
		GLushort ia = m_indicies[i];
		GLushort ib = m_indicies[i + 1];
		GLushort ic = m_indicies[i + 2];
		vec3 norm = -normalize(cross(vec3(m_verticies[ic].position) - vec3(m_verticies[ia].position),
			vec3(m_verticies[ib].position) - vec3(m_verticies[ia].position)));
		int v[3];  v[0] = ia;  v[1] = ib;  v[2] = ic;
		for (int j = 0; j < 3; j++) {
			GLushort cur_v = v[j];
			seen[cur_v]++;
			if (seen[cur_v] == 1) {
				m_verticies[cur_v].normal = norm;
			}
			else {
				// average
				m_verticies[cur_v].normal.x = m_verticies[cur_v].normal.x * (1.0f - 1.0f / seen[cur_v]) + norm.x * 1.0f / seen[cur_v];
				m_verticies[cur_v].normal.y = m_verticies[cur_v].normal.y * (1.0f - 1.0f / seen[cur_v]) + norm.y * 1.0f / seen[cur_v];
				m_verticies[cur_v].normal.z = m_verticies[cur_v].normal.z * (1.0f - 1.0f / seen[cur_v]) + norm.z * 1.0f / seen[cur_v];
				m_verticies[cur_v].normal = normalize(m_verticies[cur_v].normal);
			}
		}

	}
}