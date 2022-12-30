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

struct Move {
	Piece* piece;
	unsigned int startingSquare;
	unsigned int targetSquare;
};

class Scene {
public:
	GLFWwindow* m_Window;
	Camera m_Camera;
	Light m_Light;
	Material m_Wood;
	glm::mat4 m_SceneProjectionMatrix;
	std::string m_TexturesDirectory;
	std::string m_ShadersDirectory;

	bool m_MouseWasPressed;

	Board* m_Board;
	Piece* m_ActivePiece;
	Piece* m_PieceToMove;
	Square* m_SquareToMoveTo;
	Color m_ColorToMove;
	Scene() = default;
	Scene(GLFWwindow* window, Camera camera, Light light, glm::mat4 sceneProjectionMatrix, std::string texturesDirectory, std::string shadersDirectory);

	void Draw(glm::vec3 mouseRay, bool mouseIsDown);
private:
	int m_DirectionOffsets[8] = { 1, -1, -8, 8, -7, 7, 9, -9 };
	int m_NumSquaresToEdge[64][8];

	void precomputeMoveData();

	void unselectOldPiece();
	void unselectOldSquare();
	void findLegalSquares(Piece* piece, std::vector<Move> moves);
	Piece* findSelectedPiece(glm::vec3 mouseRay);
	Square* findSelectedSquare(glm::vec3 mouseRay);
	Square* findOldSquare();

	void movePiece();
	void nextTurn();

	std::vector<Move> generateMoves();
	void generateSlidingMoves(unsigned int start, Piece* piece, std::vector<Move> &moves);
};

#endif