#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Light.h"
#include "Board.h"


class Scene {
public:
	GLFWwindow* m_Window;
	Camera m_Camera;
	Light m_Light;
	Material m_Wood;
	glm::mat4 m_SceneProjectionMatrix;
	std::string m_TexturesDirectory;
	std::string m_ShadersDirectory;

	Board m_Board;
	Scene() = default;
	Scene(GLFWwindow* window, Camera camera, Light light, glm::mat4 sceneProjectionMatrix, std::string texturesDirectory, std::string shadersDirectory);

	void Draw();
private:
	Board createBoard();
};

#endif