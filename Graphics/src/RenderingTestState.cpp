#include "RenderingTestState.h"
#include "FlyCamera.h"
#include "ShaderLoader.h"
#include <gl_core_4_4.h>
#include "GLFW\glfw3.h"
#include <stb-master\stb_image.h>
#include "Debug.h"
void RenderingTestState::Init(GLFWwindow* _window, GameStateManager* _gameStateManager) {
	m_window = _window;
	m_gameStateManager = _gameStateManager;
	m_freeCamera = new FlyCamera(100.0f);
	m_freeCamera->SetInputWindow(m_window);
	m_freeCamera->SetPerspective(0.9f, 16.0f / 9.0f, 0.1f, 10000.0f);
	m_freeCamera->SetLookAt(glm::vec3(300, 100, 300), glm::vec3(0), glm::vec3(0, 1, 0));
	m_freeCamera->SetRotationSpeed(0.2f);

	m_programID = ShaderLoader::LoadProgram("res/shaders/raymarch.vs", "res/shaders/raymarch.fs");

	glUseProgram(m_programID);

	// A quad extending from -1 to 1 on the x and y axis (the entire screen)
	GLfloat vertices[] = {
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f, -1.0f,
		1.0f,  1.0f
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint positionAttrib = glGetAttribLocation(m_programID, "position");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	m_resolutionLoc	= glGetUniformLocation(m_programID, "m_resolution");
	m_camUpLoc		= glGetUniformLocation(m_programID, "m_camUp");
	m_camRightLoc	= glGetUniformLocation(m_programID, "m_camRight");
	m_camForwardLoc	= glGetUniformLocation(m_programID, "m_camForward");
	m_eyeLoc			= glGetUniformLocation(m_programID, "m_eye");
	m_focalLengthLoc = glGetUniformLocation(m_programID, "m_focalLength");
	m_zNearLoc		= glGetUniformLocation(m_programID, "m_zNear");
	m_zFarLoc		= glGetUniformLocation(m_programID, "m_zFar");
	m_aspectRatioLoc = glGetUniformLocation(m_programID, "m_aspectRatio");
	m_rmStepsLoc		= glGetUniformLocation(m_programID, "m_rmSteps");
	m_rmEpsilonLoc	= glGetUniformLocation(m_programID, "m_rmEpsilon");
	m_skyColorLoc	= glGetUniformLocation(m_programID, "m_skyColor");
	m_ambientLoc		= glGetUniformLocation(m_programID, "m_ambient");
	m_light0PosLoc	= glGetUniformLocation(m_programID, "m_light0Position");
	m_light0ColorLoc = glGetUniformLocation(m_programID, "m_light0Color");

	glUniform2f(m_resolutionLoc, 1320, 800);
	glUniform1f(m_zNearLoc, m_zNear);
	glUniform1f(m_zFarLoc, m_zFar);
	glUniform1f(m_aspectRatioLoc, m_aspectRatio);
	glUniform1f(m_rmEpsilonLoc, m_rmEpsilon);
	glUniform1i(m_rmStepsLoc, m_rmSteps);

	double renderTime = 0.0;
	double targetFrameTime = 1.0 / 30.0;
}
RenderingTestState::~RenderingTestState()
{
}

void RenderingTestState::Update(double _dt) {
	
	UpdateCamera(_dt);
	GUI();
}
void RenderingTestState::Draw()
{

	glUseProgram(m_programID);
//
	// A quad extending from -1 to 1 on the x and y axis (the entire screen)
	GLfloat vertices[] = {
		-1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f, -1.0f,
		1.0f,  1.0f
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint positionAttrib = glGetAttribLocation(m_programID, "position");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	m_resolutionLoc = glGetUniformLocation(m_programID, "m_resolution");
	m_camUpLoc = glGetUniformLocation(m_programID, "m_camUp");
	m_camRightLoc = glGetUniformLocation(m_programID, "m_camRight");
	m_camForwardLoc = glGetUniformLocation(m_programID, "m_camForward");
	m_eyeLoc = glGetUniformLocation(m_programID, "m_eye");
	m_focalLengthLoc = glGetUniformLocation(m_programID, "m_focalLength");
	m_zNearLoc = glGetUniformLocation(m_programID, "m_zNear");
	m_zFarLoc = glGetUniformLocation(m_programID, "m_zFar");
	m_aspectRatioLoc = glGetUniformLocation(m_programID, "m_aspectRatio");
	m_rmStepsLoc = glGetUniformLocation(m_programID, "m_rmSteps");
	m_rmEpsilonLoc = glGetUniformLocation(m_programID, "m_rmEpsilon");
	m_skyColorLoc = glGetUniformLocation(m_programID, "m_skyColor");
	m_ambientLoc = glGetUniformLocation(m_programID, "m_ambient");
	m_light0PosLoc = glGetUniformLocation(m_programID, "m_light0Position");
	m_light0ColorLoc = glGetUniformLocation(m_programID, "m_light0Color");
//
//	glUniform2f(m_resolutionLoc, 1320, 800);
//	glUniform1f(m_zNearLoc, m_zNear);
//	glUniform1f(m_zFarLoc, m_zFar);
//	glUniform1f(m_aspectRatioLoc, m_aspectRatio);
//	glUniform1f(m_rmEpsilonLoc, m_rmEpsilon);
	glUniform1i(m_rmStepsLoc, m_rmSteps);
//
	glUniform3fv(m_camUpLoc, 1, value_ptr(m_camUp));
	glUniform3fv(m_camRightLoc, 1, value_ptr(m_camRight));
	glUniform3fv(m_camForwardLoc, 1, value_ptr(m_camForward));
	glUniform3fv(m_eyeLoc, 1, value_ptr(m_eye));
	glUniform1f(m_focalLengthLoc, m_focalLength);
	glUniform4fv(m_skyColorLoc, 1, value_ptr(m_skyColor));
	glUniform4fv(m_ambientLoc, 1, value_ptr(m_ambient));
	glUniform3fv(m_light0PosLoc, 1, value_ptr(m_light0Position));
	glUniform4fv(m_light0ColorLoc, 1, value_ptr(m_light0Color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}
void RenderingTestState::GUI() 
{
	ImGui::SliderInt("Raymarch steps", &m_rmSteps, 0, 1000);
	ImGui::ColorEdit4("Sky Color", glm::value_ptr(m_skyColor));
	ImGui::ColorEdit4("Ambient Color", glm::value_ptr(m_ambient));
	ImGui::SliderFloat3("Light Position", glm::value_ptr(m_light0Position), -10.0f, 10.0f);
	ImGui::ColorEdit4("Light Color", glm::value_ptr(m_light0Color));
}
void RenderingTestState::UpdateCamera(float dt)
{
	if (glfwGetKey(m_window, GLFW_KEY_A))
		m_eye -= m_camRight * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_D))
			m_eye += m_camRight * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_W))
		m_eye += m_camForward * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_S))
		m_eye -= m_camForward * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
		m_eye += m_camUp * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL))
		m_eye -= m_camUp * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_LEFT))
		m_light0Position -= m_camRight * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_RIGHT))
		m_light0Position += m_camRight * m_moveSpeed;

	if (glfwGetKey(m_window, GLFW_KEY_UP))
		m_light0Position += m_camUp * m_moveSpeed;
	else if (glfwGetKey(m_window, GLFW_KEY_DOWN))
		m_light0Position -= m_camUp * m_moveSpeed;


		if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2))
		{
	double mposX, mposY;
	
	glfwGetCursorPos(m_window, &mposX, &mposY);
	glfwSetCursorPos(m_window, m_windowWidth / 2, m_windowHeight / 2);
	int dx = mposX - m_windowWidth / 2;
	int dy = mposY - m_windowHeight / 2;
	m_theta += dx * 0.01f;
	static const float TWO_PI = 6.28318530718f;
	if (m_theta > TWO_PI) m_theta -= TWO_PI;
	else if (m_theta < 0.0f) m_theta += TWO_PI;

	m_phi += dy * 0.01f;
	if (m_phi > TWO_PI) m_phi -= TWO_PI;
	else if (m_phi < 0.0f) m_phi += TWO_PI;

	float sintheta = sinf(m_theta);
	float costheta = cosf(m_theta);
	float sinphi = sinf(m_phi);
	float cosphi = cosf(m_phi);
	m_camForward = glm::vec3(cosphi * sintheta, -sinphi, cosphi * costheta);
	m_camRight = glm::vec3(costheta, 0.0f, -sintheta);
	m_camUp = glm::normalize(cross(m_camForward, m_camRight));
	}
}