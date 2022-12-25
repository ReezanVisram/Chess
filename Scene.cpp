#include "Scene.h"
Scene::Scene(GLFWwindow* window, Camera camera, Light light, glm::mat4 sceneProjectionMatrix, std::string texturesDirectory, std::string shadersDirectory) {
	m_Window = window;
	m_Camera = camera;
	m_Light = light;
	m_SceneProjectionMatrix = sceneProjectionMatrix;
	m_TexturesDirectory = texturesDirectory;
	m_ShadersDirectory = shadersDirectory;

	m_Wood = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5, 0.5f),
		4.0
	};

	m_Board = new Board(m_TexturesDirectory, glm::vec3(-4.0f, -3.0f, 0.0f), m_ShadersDirectory, &m_SceneProjectionMatrix, &m_Camera, &m_Light, &m_Wood);
}

void Scene::Draw(glm::vec3 mouseRay) {
	m_Board->Draw(mouseRay);
}