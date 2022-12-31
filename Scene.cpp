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
	m_ActivePiece->m_HasMoved = true;
	m_ActivePiece->m_NumMoves++;
	switch (m_EnPassant) {
	case None:
		break;
	case WhiteLeft:
		m_Board->m_Squares[m_SquareToMoveTo->m_File * 8 + m_SquareToMoveTo->m_Rank - 1].m_Piece = nullptr;
		break;
	case WhiteRight:
		m_Board->m_Squares[m_SquareToMoveTo->m_File * 8 + m_SquareToMoveTo->m_Rank - 1].m_Piece = nullptr;
		break;
	case BlackLeft:
		m_Board->m_Squares[m_SquareToMoveTo->m_File * 8 + m_SquareToMoveTo->m_Rank + 1].m_Piece = nullptr;
		break;
	case BlackRight:
		m_Board->m_Squares[m_SquareToMoveTo->m_File * 8 + m_SquareToMoveTo->m_Rank + 1].m_Piece = nullptr;
		break;
	}
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
	m_EnPassant = None;
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
	int knightOffsets[] = { -6, 6, -15, 15, -10, 10, -17, 17 };

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

			// Knight
			unsigned int knightSquares[8] = {};
			unsigned int numPossibleKnightSquares = 0;
			for (int knightJump = 0; knightJump < 8; knightJump++) {
				int target = squareIndex + knightOffsets[knightJump];
				if (target >= 0 && target < 64) {
					int knightSquareX = target / 8;
					int knightSquareY = target - knightSquareX * 8;
					int maxMoveDistance = std::max(std::abs(file - knightSquareX), std::abs(rank - knightSquareY));
					if (maxMoveDistance == 2) {
						knightSquares[numPossibleKnightSquares] = target;
						numPossibleKnightSquares++;
					}
				}
			}

			for (int i = 0; i < 8; i++) {
				m_KnightMoves[squareIndex][i] = knightSquares[i];
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
				else if (piece->m_Type == Knight) {
					generateKnightMoves(start, piece, moves);
				}
				else if (piece->m_Type == Pawn) {
					generatePawnMoves(start, piece, moves);
				}
				else if (piece->m_Type == King) {
					generateKingMoves(start, piece, moves);
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
			int targetSquare = start + m_SlidingDirectionOffsets[direction] * (n + 1);

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

void Scene::generateKnightMoves(unsigned int start, Piece* piece, std::vector<Move>& moves) {
	for (int n = 0; n < 8; n++) {
		if (m_KnightMoves[start][n] != 0) {
			int targetSquare = m_KnightMoves[start][n];

			Piece* pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;

			if ((pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_Color != piece->m_Color) || pieceOnTargetSquare == nullptr) {
				Move move = { piece, start, targetSquare };
				moves.push_back(move);
			}


		}
	}
}

void Scene::generatePawnMoves(unsigned int start, Piece* piece, std::vector<Move>& moves) {
	int moveDir = 0;
	if (piece->m_Color == White) {
		moveDir = 1;
	}
	else {
		moveDir = -1;
	}

	// 1 straight move
	Piece* pieceOnTargetSquare = m_Board->m_Squares[start + moveDir].m_Piece;
	if (pieceOnTargetSquare == nullptr) {
		Move move = { piece, start, start + moveDir };
		moves.push_back(move);
	}

	// 2 straight on first movve
	pieceOnTargetSquare = m_Board->m_Squares[start + moveDir * 2].m_Piece;
	if (pieceOnTargetSquare == nullptr && !piece->m_HasMoved) {
		Move move = { piece, start, start + moveDir * 2 };
		moves.push_back(move);
	}

	// Check captures
	int targetSquare = start + 9 * moveDir;
	if (targetSquare >= 0 && targetSquare < 63) {
		pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;
		if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_Color != piece->m_Color) {
			Move move = { piece, start, targetSquare };
			moves.push_back(move);
		}
	}
	targetSquare = start - 7 * moveDir;
	if (targetSquare >= 0 && targetSquare < 63) {
		pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;
		if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_Color != piece->m_Color) {
			Move move = { piece, start, targetSquare };
			moves.push_back(move);
		}
	}

	// En passant (White)
	if (piece->m_Color == White && start % 8 == 4) {
		// Left
		targetSquare = start - 8;
		if (targetSquare > 0) {
			pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;

			if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_NumMoves == 1) {
				Move move = { piece, start, start - 7 };
				m_EnPassant = WhiteLeft;
				moves.push_back(move);
			}
		}
		targetSquare = start + 8;
		if (targetSquare < 63) {
			pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;

			if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_NumMoves == 1) {
				Move move = { piece, start, start + 9 };
				m_EnPassant = WhiteRight;
				moves.push_back(move);
			}
		}
	}

	// En passant (Black)
	if (piece->m_Color == Black && start % 8 == 3) {
		// Left
		targetSquare = start - 8;
		if (targetSquare > 0) {
			pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;

			if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_NumMoves == 1) {
				Move move = { piece, start, start - 9 };
				m_EnPassant = BlackLeft;
				moves.push_back(move);
			}
		}
		targetSquare = start + 8;
		if (targetSquare < 63) {
			pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;

			if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_NumMoves == 1) {
				Move move = { piece, start, start + 7 };
				m_EnPassant = BlackRight;
				moves.push_back(move);
			}
		}
	}

}

void Scene::generateKingMoves(unsigned int start, Piece* piece, std::vector<Move>& moves) {
	for (int direction = 0; direction < 8; direction++) {
		int targetSquare = start + m_SlidingDirectionOffsets[direction];

		if (targetSquare >= 0 && targetSquare < 63) {
			Piece* pieceOnTargetSquare = m_Board->m_Squares[targetSquare].m_Piece;
			if (pieceOnTargetSquare != nullptr && pieceOnTargetSquare->m_Color == piece->m_Color) {
				break;
			}

			Move move = { piece, start, targetSquare };
			moves.push_back(move);
		}
	}
}