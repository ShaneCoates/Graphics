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
#include "Mesh.h"
#include "Material.h"
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

void ObjectOBJ::Update(double _dt)
{
	for each (Mesh* mesh in m_submeshes)
	{
		mesh->Update(_dt);
	}
}

void ObjectOBJ::Draw(Camera* _camera)
{
	for each (Mesh* mesh in m_submeshes)
	{
		mesh->Draw(_camera, m_MVP, m_texture);
	}
}

void ObjectOBJ::LoadFromObj(const char* _path)
{

	std::vector<Vertex> aoVerticies;
	std::vector<unsigned int> auiIndicies;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<int> seen;

	std::string mtlPath;

	std::ifstream in(_path, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << _path << std::endl; }
	bool fileContrainsNormals = false;
	std::string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			m_submeshes.back()->AddVert(line);
		}
		else if (line.substr(0, 2) == "f ")
		{

			if (fileContrainsNormals) 
			{
				m_submeshes.back()->AddFaceWithNormals(line, texCoords, normals);
			}
			else 
			{
				m_submeshes.back()->AddFace(line, texCoords);
			}
			m_indexCount += 3;

		}
		else if (line.substr(0, 3) == "vn ")
		{
			fileContrainsNormals = true;
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
		else if (line.substr(0, 2) == "g ")
		{
			if (m_submeshes.size() > 0)
			{
				if (!fileContrainsNormals)
					m_submeshes.back()->GenerateNormals();
				m_submeshes.back()->FinishImport();
			}
			m_submeshes.push_back(new Mesh());
			m_submeshes.back()->BeginImport(m_programID);
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}
	if (m_submeshes.size() > 0)
	{
		if (!fileContrainsNormals)
			m_submeshes.back()->GenerateNormals();
		m_submeshes.back()->FinishImport();
	}
	LoadTexture(mtlPath.c_str());
}

void ObjectOBJ::LoadTexture(const char* _path)
{
	std::ifstream in(_path, std::ios::in);
	if (!in) { std::cerr << "Cannot open " << _path << std::endl;  return; }

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
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
