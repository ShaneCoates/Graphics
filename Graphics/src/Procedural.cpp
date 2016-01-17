#include "Procedural.h"
#include "FlyCamera.h"
#include "SpinningCamera.h"
#include "ShaderLoader.h"
#include <gl_core_4_4.h>
#include "GLFW\glfw3.h"
#include <stb-master\stb_image.h>
#include "Skybox.h"
#include "aieutilities\Gizmos.h"
#include "Terrain.h"
#include <iostream>
#include <sstream>
#include "ParticleEmitter.h"
#include "ObjectOBJ.h"
void Procedural::Init(GLFWwindow* _window, GameStateManager* _gameStateManager) {
	m_window = _window;
	m_gameStateManager = _gameStateManager;
	m_freeCamera = new FlyCamera(100.0f);
	m_freeCamera->SetInputWindow(m_window);
	m_freeCamera->SetPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	m_freeCamera->SetLookAt(glm::vec3(300, 100, 300), glm::vec3(0), glm::vec3(0, 1, 0));
	m_spinCamera = new SpinningCamera(100.0f);
	m_spinCamera->SetPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	m_activeCamera = m_spinCamera;
	m_skybox = new Skybox();
	m_terrain = new Terrain();
	m_terrain->NewSeed();
	m_seedCount = 0;
	m_seeds.push_back(m_terrain->m_seed);
	m_ringEmitter = new ParticleEmitter();
	m_ringEmitter->Init(50000, 50.0f, 100.0f, 1.0f, 5.0f, 0.1f, 0.15f, glm::vec4(0.3f, 0.3f, 0.5f, 0.75f), glm::vec4(0.5f, 0.5f, 0.3f, 0.25f));
	m_ringEmitter->CreateUpdateShader("res/shaders/gpuParticleUpdateRing.vs");
	m_ringEmitter->m_emitterPosition = glm::vec3(10, 60, 120);
	const char* path[2];
	path[0] = "res/textures/circle.png";
	path[1] = "res/textures/star.png";
	m_ringEmitter->LoadTexture(path);
	m_starsEmitter = new ParticleEmitter();
	m_starsEmitter->Init(50000, 50.0f, 100.0f, 1.0f, 5.0f, 0.1f, 0.15f, glm::vec4(1.0f, 1.0f, 1.0f, 0.75f), glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
	m_starsEmitter->CreateUpdateShader("res/shaders/gpuParticleUpdate.vs");
	m_starsEmitter->LoadTexture(path);
	unsigned int m_program = ShaderLoader::LoadProgram("res/shaders/simpleOBJ.vs", "res/shaders/simpleOBJ.fs");
	m_planet1 = new ObjectOBJ("res/models/AlienPlanet.obj", m_program);
	m_planet1->Translate(glm::vec3(10, 50, 120));
	m_planet1->SetScale(glm::vec3(8, 8, 8));
	m_planet2 = new ObjectOBJ("res/models/AlienPlanet2.obj", m_program);
	m_planet2->Translate(glm::vec3(150, -30, 20));
	m_planet2->SetScale(glm::vec3(4, 4, 4));
	m_freeCam = false;
}
void Procedural::Update(double _dt) {
	if (m_freeCam) {
		m_activeCamera = m_freeCamera;
	} else {
		m_activeCamera = m_spinCamera;
	}

	m_activeCamera->Update(_dt);
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		m_terrain->ReloadShaders();
		m_ringEmitter->CreateDrawShader();
		m_ringEmitter->CreateUpdateShader("res/shaders/gpuParticleUpdateRing.vs");
		m_starsEmitter->CreateDrawShader();
		m_starsEmitter->CreateUpdateShader("res/shaders/gpuParticleUpdate.vs");

	}
	GUI();
	m_planet1->m_lightHeight = m_terrain->m_lightHeight;
	m_planet2->m_lightHeight = m_terrain->m_lightHeight;
	m_planet1->Update(_dt);
	m_planet2->Update(_dt);
}
void Procedural::Draw() {
	m_skybox->Draw(m_activeCamera);
	m_ringEmitter->Draw((float)glfwGetTime(), m_activeCamera);
	m_starsEmitter->Draw((float)glfwGetTime(), m_activeCamera);
	m_planet1->Draw(m_activeCamera);
	m_planet2->Draw(m_activeCamera);
	m_terrain->Draw(m_activeCamera);
}
void Procedural::GUI() {
	ImGui::Checkbox("Free Camera", &m_freeCam);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Light")) {
		ImGui::SliderFloat("Height", &m_terrain->m_lightHeight, -2, 2);
	}
	if (ImGui::CollapsingHeader("Realtime Terrain")) {
		ImGui::SliderFloat("Sea Level", &m_terrain->m_amplitude, 0, 2);
		ImGui::SliderFloat("Size", &m_terrain->m_size, 0.1f, 2);
	}
	if (ImGui::CollapsingHeader("Reload Terrain")) {
		ImGui::SliderInt("Seed", &m_terrain->m_seed, 0, 100000);
		ImGui::SliderInt("Octaves", &m_terrain->m_octaves, 1, 10);
		ImGui::SliderFloat("Persistence", &m_terrain->m_persistence, 0, 1);
		if (ImGui::Button("Use Seed")) {
			m_terrain->NewSeed(m_terrain->m_seed);
			m_seeds.push_back(m_terrain->m_seed);
		}
		ImGui::SameLine(0, (int)ImGui::GetStyle().ItemInnerSpacing.x);
		if (ImGui::Button("Random Seed")) {
			m_terrain->NewSeed();
			m_seeds.push_back(m_terrain->m_seed);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("Sub2", ImVec2(0, 300), true);
		ImGui::Text("Recent Seeds");
		ImGui::Columns(3);
		for (unsigned int i = 0; i < m_seeds.size(); i++)
		{
			if (i > 0)
				ImGui::NextColumn();

			std::stringstream temp_str;
			temp_str << (m_seeds[i]);
			std::string str = temp_str.str();
			const char* cstr2 = str.c_str();
			if (ImGui::Button(cstr2)) {
				m_terrain->NewSeed(m_seeds[i]);
			}

		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

}
