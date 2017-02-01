/*
Author: Shane Coates
Description: Test state for raymarching
*/

#ifndef _RENDERING_TEST_STATE_H_
#define	_RENDERING_TEST_STATE_H_
#include "GameStateManager.h"
#include "glm.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "gl_core_4_4.h"
#include "glfw\glfw3.h"

struct GLFWwindow;
class FlyCamera;
class Camera;

class RenderingTestState : public IGameState
{
public:
	//Constructor inherits from IGameState and calls the Init function
	RenderingTestState(GLFWwindow* _window, GameStateManager* _gameStateManager) : IGameState()
	{
		Init(_window, _gameStateManager);
	}
	//Destructor
	~RenderingTestState();

	//Initialise the gamestate
	void Init(GLFWwindow* _window, GameStateManager* _gameStateManager);

	//Update/Draw functions
	void Update(double _dt);
	void Draw();

protected:
private:

	void GUI();
	void UpdateCamera(float dt);

	GLFWwindow* m_window;
	GameStateManager* m_gameStateManager;
	FlyCamera* m_freeCamera;

	unsigned int m_programID;

	float m_theta = 0.0f;										// Horizontal angle
	float m_phi = 0.0f;											// Vertical angle
	glm::vec3 m_camUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));	// The upward-vector of the image plane
	glm::vec3 m_camRight = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));	// The right-vector of the image plane
	glm::vec3 m_camForward = cross(m_camRight, m_camUp);			// The forward-vector of the image plane
	glm::vec3 m_eye = glm::vec3(0.0f, 0.0f, -2.0f);				// The eye position in the world
	float m_focalLength = 1.67f;								// Distance between eye and image-plane
	float m_zNear = 0.0f;									// Near plane distance from camera
	float m_zFar = 15.0f;								// Far plane distance from camera
	float m_moveSpeed = 0.1f;

	// Raymarch parameters
	int m_rmSteps = 64;
	float m_rmEpsilon = 0.001f;

	// Scene
	glm::vec4 m_skyColor = glm::vec4(0.31f, 0.47f, 0.67f, 1.0f);
	glm::vec4 m_ambient = glm::vec4(0.15, 0.2f, 0.32f, 1.0f);
	glm::vec3 m_light0Position = glm::vec3(0.25f, 2.0f, 0.0f);
	glm::vec4 m_light0Color = glm::vec4(0.67f, 0.87f, 0.93f, 1.0f);

	const int m_windowWidth = 1320;
	const int m_windowHeight = 800;
	float m_aspectRatio = m_windowWidth / (float)m_windowHeight;

	GLuint m_resolutionLoc;
	GLuint m_camUpLoc;
	GLuint m_camRightLoc;
	GLuint m_camForwardLoc;
	GLuint m_eyeLoc;
	GLuint m_focalLengthLoc;
	GLuint m_zNearLoc;
	GLuint m_zFarLoc;
	GLuint m_aspectRatioLoc;
	GLuint m_rmStepsLoc;
	GLuint m_rmEpsilonLoc;
	GLuint m_skyColorLoc;
	GLuint m_ambientLoc;
	GLuint m_light0PosLoc;
	GLuint m_light0ColorLoc;
};

#endif