#include "Material.h"
#include "ShaderLoader.h"

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
void Material::ApplyMaterialProperties()
{

}