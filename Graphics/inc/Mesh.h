#ifndef _MESH_H_
#define	_MESH_H_

#include <glm/glm.hpp>
#include <string>
#include <vector>
using namespace glm;
using namespace std;

struct Vertex
{
	vec4 position = vec4(0, 0, 0, 0);
	vec3 normal = vec3(0, 0, 0);
	vec2 texCoord = vec2(0, 0);
};

class Material;
class Camera;
class Mesh
{
public:
	Mesh();
	~Mesh();

	void Update(double _dt);
	void Draw(Camera* _camera, mat4 _MVP);

	void BeginImport(int _programID);
	void FinishImport();

	void AddVert(string _line);
	void AddFace(string _line, vector<vec2> _texCoords);
	void AddFaceWithNormals(string _line, vector<vec2> _texCoords, vector<vec3> _normals);

	void GenerateNormals();
	void SetMaterial(Material* _material);
private:

	bool m_importActive = false;

	Material* m_material;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_vertexCount;
	unsigned int m_indexCount;
	unsigned int m_programID;

	vector<Vertex> m_verticies;
	vector<unsigned int> m_indicies;
	vector<vec3> m_normals;
	vector<vec2> m_texCoords;

};

#endif