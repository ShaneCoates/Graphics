#include "SpinningCamera.h"
#include <GLFW\glfw3.h>

void SpinningCamera::Init() {
	//m_flySpeed = 10.0f;
}
void SpinningCamera::Update(double dt) {
	glm::vec3 cameraPos = glm::vec3(sinf((float)glfwGetTime() * 0.1f) * 300, 10, cosf((float)glfwGetTime() * 0.1f) * 300);
	SetLookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
