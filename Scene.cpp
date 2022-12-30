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
	m_MouseWasPressed = false;
	m_ColorToMove = White;
	precomputeMoveData();
}

void Scene::Draw(glm::vec3 mouseRay, bool mouseIsDown) {
	m_Board->Draw(mouseRay, mouseIsDown);

	if (mouseIsDown && !m_MouseWasPressed) {
		m_MouseWasPressed = true;
		if (m_ActivePiece == nullptr) {
			m_ActivePiece = findSelectedPiece(mouseRay);
			findLegalSquares(m_ActivePiece, generateMoves());
		}
		else {
			if (m_SquareToMoveTo == nullptr) {
				m_SquareToMoveTo = findSelectedSquare(mouseRay);
			}
		}
	}

	if (!mouseIsDown && m_MouseWasPressed) {
		m_MouseWasPressed = false;
		if (m_ActivePiece != nullptr && m_SquareToMoveTo != nullptr) {
			movePiece();
			nextTurn();
		}
	}
}

void Scene::movePiece() {
	Square* oldSquare = findOldSquare();

	oldSquare->m_Piece = nullptr;
	m_SquareToMoveTo->m_Piece = m_ActivePiece;
	unselectOldPiece();
	unselectOldSquare();
	m_SquareToMoveTo = nullptr;
	m_ActivePiece = nullptr;
}

Piece* Scene::findSelectedPiece(glm::vec3 mouseRay) {
	for (int i = 0; i < 64; i++) {
		Piece* piece = m_Board->m_Squares[i].m_Piece;
		if (piece != nullptr) {
			if (piece->IsSelected(mouseRay) && piece->m_Color == m_ColorToMove) {
				piece->m_GL_IsSelected = true;
				return piece;
			}
		}
	}

	return nullptr;
}

Square* Scene::findSelectedSquare(glm::vec3 mouseRay) {
	for (int i = 0; i < 64; i++) {
		Square* square = &m_Board->m_Squares[i];

		if (square->IsSelected(mouseRay) && square->m_IsLegal) {
			square->m_GL_IsSelected = true;
			return square;
		}
		else {
			square->m_GL_IsSelected = false;
		}
	}
	return nullptr;
}

void Scene::unselectOldPiece() {
	for (int i = 0; i < 64; i++) {
		Piece* piece = m_Board->m_Squares[i].m_Piece;
		if (piece != nullptr) {
			piece->m_GL_IsSelected = false;
		}
	}
}

void Scene::unselectOldSquare() {
	for (int i = 0; i < 64; i++) {
		m_Board->m_Squares[i].m_GL_IsSelected = false;
		m_Board->m_Squares[i].m_IsLegal = false;
	}
}

Square* Scene::findOldSquare() {
	for (int i = 0; i < 64; i++) {
		Square* square = &m_Board->m_Squares[i];

		if (square->m_Piece == m_ActivePiece) {
			return square;
		}
	}
}

void Scene::nextTurn() {
	if (m_ColorToMove == White) {
		m_ColorToMove = Black;
	}
	else {
		m_ColorToMove = White;
	}
}

void Scene::precomputeMoveData() {
	for (int file = 0; file < 8; file++) {
		for (int rank = 0; rank < 8; rank++) {
			unsigned int numNorth = 7 - rank;
			unsigned int numSouth = rank;
			unsigned int numWest = file;
			unsigned int numEast = 7 - file;

			unsigned int squareIndex = file * 8 + rank;

			unsigned int dirValues[8] = { numNorth, numSouth, numWest, numEast, std::min(numNorth, numWest), std::min(numSouth, numEast), std::min(numNorth, numEast), std::min(numSouth, numWest) };
			for (int i = 0; i < 8; i++) {
				m_NumSquaresToEdge[squareIndex][i] = dirValues[i];
			}
		}
	}
}

std::vector<Move> Scene::generateMoves() {
	std::vector<Move> moves = std::vector<Move>();

	for (int start = 0; start < 64; start++) {
		Piece* piece = m_Board->m_Squares[start].m_Piece;
		if (piece != nullptr) {
			if (piece->m_Color == m_ColorToMove) {
				if (piece->m_Type == Queen || piece->m_Type == Rook || piece->m_Type == Bishop) {
					generateSlidingMoves(start, piece, moves);
				}
			}
		}
	}

	return moves;
}

void Scene::findLegalSquares(Piece* piece, std::vector<Move> moves) {
	for (int i = 0; i < moves.size(); i++) {
		if (moves[i].piece == piece) {
			m_Board->m_Squares[moves[i].targetSquare].m_IsLegal = true;
		}
	}
}

void Scene::generateSlidingMoves(unsigned int start, Piece* piece, std::vector<Move> &moves) {
	int startDirIndex = (piece->m_Type == Bishop) ? 4 : 0;
	int endDirIndex = (piece->m_Type == Rook) ? 4 : 8;

	for (int direction = startDirIndex; direction < endDirIndex; direction++) {
		for (int n = 0; n < m_NumSquaresToEdge[start][direction]; n++) {
			int targetSquare = start + m_DirectionOffsets[direction] * (n + 1);

			Piece* pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;

			if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_Color == piece->m_Color) {
				break;
			}

			Move move = { piece, start, targetSquare };
			moves.push_back(move);

			if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_Color != piece->m_Color) {
				break;
			}
		}
	}
}