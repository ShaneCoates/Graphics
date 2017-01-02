#include "Material.h"
#include "ShaderLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace glm;
Material::Material()
{

}

Material::~Material()
{

}

void Material::SetShader(unsigned int _shaderID)
{
	m_shaderID = _shaderID;
}

void Material::SetShader(const char* a_szVertexShader, const char* a_szPixelShader, const char* a_szGeometryShader, const char* a_szTessellationControlShader, const char* a_szTessellationEvaluationShader)
{
	m_shaderID = ShaderLoader::LoadProgram(a_szVertexShader, a_szPixelShader, a_szGeometryShader, a_szTessellationControlShader, a_szTessellationEvaluationShader);
}

void Material::GenerateProperties(const char* _path)
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
		else if (line.substr(0, 7) == "newmtl ")
		{
			std::istringstream s(line.substr(7));
			s >> m_name;
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}

	if (path.length() > 1)
	{
		std::string data = "res/models/" + path;
		int textureWidth, textureHeight, textureFormat;
		unsigned char* imageData = stbi_load(data.c_str(), &textureWidth, &textureHeight, &textureFormat, STBI_default);


		glGenTextures(1, &m_mainTex);
		glBindTexture(GL_TEXTURE_2D, m_mainTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		stbi_image_free(imageData);
	}
}

void Material::ApplyMaterialProperties(unsigned int _programID)
{
	unsigned int ambientColorUniform = glGetUniformLocation(_programID, "ambientColor");
	unsigned int diffuseColorUniform = glGetUniformLocation(_programID, "diffuseColor");
	unsigned int specColorUniform = glGetUniformLocation(_programID, "specColor");

	glUniform3fv(ambientColorUniform, 1, value_ptr(ambientColor));
	glUniform3fv(diffuseColorUniform, 1, value_ptr(diffuseColor));
	glUniform3fv(specColorUniform, 1, value_ptr(specColor));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, GetTextureID());

}