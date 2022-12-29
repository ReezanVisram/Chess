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

	Board* m_Board;
	Piece* m_ActivePiece;
	Square* m_SquareToMoveTo;
	Scene() = default;
	Scene(GLFWwindow* window, Camera camera, Light light, glm::mat4 sceneProjectionMatrix, std::string texturesDirectory, std::string shadersDirectory);

	void Draw(glm::vec3 mouseRay, bool mouseIsDown);
	void unselectOldPiece();
	void unselectOldSquare();
	Piece* findSelectedPiece(glm::vec3 mouseRay);
	Square* findSelectedSquare(glm::vec3 mouseRay);
	Square* findOldSquare(Piece* piece);

	void movePiece();
};

#endif