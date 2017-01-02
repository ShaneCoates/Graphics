#ifndef _MATERIAL_H_
#define	_MATERIAL_H_
#include <glm/ext.hpp>
#include <string>
class Material
{
public:
	Material();
	~Material();


	void SetShader(unsigned int _shaderID);
	void SetShader(const char* a_szVertexShader, const char* a_szPixelShader, const char* a_szGeometryShader = nullptr, const char* a_szTessellationControlShader = nullptr, const char* a_szTessellationEvaluationShader = nullptr);

	void GenerateProperties(const char* _path);

	void ApplyMaterialProperties(unsigned int _programID);

	std::string GetMaterialName()
	{
		return m_name;
	}
	unsigned int GetTextureID()
	{
		return m_mainTex;
	}

private:

	std::string m_name;

	unsigned int m_shaderID;
	unsigned int m_mainTex;

	glm::vec3 diffuseColor;
	glm::vec3 ambientColor;
	glm::vec3 specColor;
};

#endif