/* 
	Author: Shane Coates
	Description: Test state for shadows
*/

#ifndef _MAIN_STATE_H_
#define	_MAIN_STATE_H_
#include "GameStateManager.h"
#include "glm.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))

struct GLFWwindow;
class FlyCamera;
class Camera;
class SpinningCamera;
class ObjectOBJ;
class PerlinParticleEmitter;
class Skybox;
class Terrain;
class ParticleEmitter;
struct Vertex{ 
	glm::vec4 Position;
	glm::vec2 UV;
};
class MainState : public IGameState
{
public:
	//Constructor inherits from IGameState and calls the Init function
	MainState(GLFWwindow* _window, GameStateManager* _gameStateManager) : IGameState()
	{
		Init(_window, _gameStateManager);
	}
	//Destructor
	~MainState();

	//Initialise the gamestate
	void Init(GLFWwindow* _window, GameStateManager* _gameStateManager);

	//Update/Draw functions
	void Update(double _dt);
	void Draw();

protected:
private:

	void GUI();

	GLFWwindow* m_window;
	GameStateManager* m_gameStateManager;
	FlyCamera* m_freeCamera;
	SpinningCamera* m_spinCamera;
	Camera* m_activeCamera;
	ObjectOBJ* m_planet1;

	Skybox* m_skybox;

	bool m_freeCam;
};

#endif