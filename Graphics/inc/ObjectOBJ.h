/*
Author: Shane Coates
Description: Object class loaded from OBJ
*/

#ifndef _OBJECT_OBJ_H
#define	_OBJECT_OBJ_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <map>
class Material;
class Camera;
class Mesh;
class ObjectOBJ
{
public:
	ObjectOBJ(unsigned int _programID);
	ObjectOBJ(const char* _path, unsigned int _programID);
	~ObjectOBJ();

	void Init(unsigned int _programID);

	void Update(double dt);
	void Draw(Camera* _camera);

	void LoadFromObj(const char* _path);
	void LoadTexture(const char* _path);


	void Translate(glm::vec3 _translation);
	void SetScale(glm::vec3 _scale);
	void Rotate(glm::vec3 _facing);


	glm::vec3 GetPosition();
	glm::mat4 GetTransform() const			{ return m_MVP; }

	float m_lightHeight = 0.0f;

protected:
private:
	

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_vertexCount;
	unsigned int m_indexCount;
	unsigned int m_programID;

	unsigned int m_texture;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specColor;

	glm::mat4 m_MVP;

	glm::vec3 m_scale;

	std::vector<Mesh*> m_submeshes;
	std::map<std::string, Material*> m_materials;
};

#endif