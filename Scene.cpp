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
	m_ActivePiece = nullptr;
	m_SquareToMoveTo = nullptr;
}

void Scene::Draw(glm::vec3 mouseRay, bool mouseIsDown) {
	m_Board->Draw(mouseRay, mouseIsDown);

	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_PRESS) {
		std::cout << "Mouse was pressed!\n";
	}
}

void Scene::movePiece() {
	std::cout << "Moving piece!\n";
}

Piece* Scene::findSelectedPiece(glm::vec3 mouseRay) {
	for (int i = 0; i < 64; i++) {
		Piece* piece = m_Board->m_Squares[i].m_Piece;
		if (piece != nullptr) {
			if (piece->IsSelected(mouseRay)) {
				piece->m_GL_IsSelected = true;
				return piece;
			}
		}
	}

	return nullptr;
}

void Scene::unselectOldPiece() {
	for (int i = 0; i < 64; i++) {
		Piece* piece = m_Board->m_Squares[i].m_Piece;

		if (piece != nullptr) {
			if (piece != m_ActivePiece) {
				piece->m_GL_IsSelected = false;
			}
		}

	}
}

Square* Scene::findSelectedSquare(glm::vec3 mouseRay) {
	for (int i = 0; i < 64; i++) {
		Square* square = &m_Board->m_Squares[i];

		if (square->IsSelected(mouseRay)) {
			square->m_GL_IsSelected = true;
			return square;
		}
		else {
			square->m_GL_IsSelected = false;
		}
	}
	return nullptr;
}

void Scene::unselectOldSquare() {
	for (int i = 0; i < 64; i++) {
		Square* square = &m_Board->m_Squares[i];

		if (square != nullptr) {
			if (square != m_SquareToMoveTo) {
				square->m_GL_IsSelected = false;
			}
		}
	}
}

Square* Scene::findOldSquare(Piece* piece) {
	for (int i = 0; i < 64; i++) {
		Square* square = &m_Board->m_Squares[i];

		if (square->m_Piece == piece) {
			return square;
		}
	}
}