#ifndef _MATERIAL_H_
#define	_MATERIAL_H_

class Material
{
public:
	Material();
	~Material();

	void SetShader(unsigned int _shaderID);
	void SetShader(const char* a_szVertexShader, const char* a_szPixelShader, const char* a_szGeometryShader = nullptr, const char* a_szTessellationControlShader = nullptr, const char* a_szTessellationEvaluationShader = nullptr);

	void ApplyMaterialProperties();

private:
	unsigned int m_shaderID;
};

#endif