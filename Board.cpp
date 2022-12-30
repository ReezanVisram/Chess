#include <stb_image.h>
#include <map>

#include "Board.h"

Board::Board(std::string texturesDirectory, glm::vec3 startingPoint, std::string shadersDirectory, glm::mat4 *projectionMatrix, Camera* camera, Light* light, Material* material) {
	m_TexturesDirectory = texturesDirectory;
	m_ShadersDirectory = shadersDirectory;
	m_GL_ViewMatrix = camera->getViewMatrix();
	m_GL_StartingPoint = startingPoint;
	m_GL_Camera = camera;
	m_GL_Light = light;
	m_GL_Material = material;
	m_GL_ProjectionMatrix = projectionMatrix;

	int i = 0;
	for (unsigned int file = 0; file < 8; file++) {
		for (unsigned int rank = 0; rank < 8; rank++) {
			bool isLight = (file + rank) % 2 != 0;

			m_Squares[i] = Square((m_TexturesDirectory + "/dark.jpg").c_str(), (m_TexturesDirectory + "/light.jpg").c_str(), (m_ShadersDirectory + "/square.vert").c_str(), (m_ShadersDirectory + "/square.frag").c_str(), file, rank, isLight, glm::vec3(startingPoint.x + file, startingPoint.y + rank, startingPoint.z), m_GL_ProjectionMatrix, m_GL_Camera, m_GL_Light, m_GL_Material);
			i++;
		}
	}
	LoadPositionFromFen("B3Q////q///b");
}

void Board::Draw(glm::vec3 mouseRay, bool mouseIsDown) {
	for (int i = 0; i < 64; i++) {
		m_Squares[i].Draw(mouseRay, mouseIsDown);
	}
}

void Board::LoadPositionFromFen(std::string fenString) {
	std::map<char, Type> charToPiece = { {'r', Rook}, {'n', Knight}, {'b', Bishop}, {'q', Queen}, {'k', King}, {'p', Pawn} };

	int file = 0;
	int rank = 7;

	int i = 0;
	std::string delimiter = " ";
	std::string fen = fenString.substr(0, fenString.find(delimiter)); 
	for (auto& symbol : fen) {
		if (symbol == '/') {
			file = 0;
			rank--;
		}
		else {
			if (isdigit(symbol)) {
				file += symbol - '0';
			}
			else {
				Color pieceColor = Black;
				if (isupper(symbol)) {
					pieceColor = White;
				}
				Type pieceType = charToPiece[tolower(symbol)];
				m_Pieces[i] = Piece((m_ShadersDirectory + "/chesset.vert").c_str(), (m_ShadersDirectory + "/chesset.frag").c_str(), (m_ShadersDirectory + "/outline.vert").c_str(), (m_ShadersDirectory + "/outline.frag").c_str(), pieceColor, pieceType, m_GL_ProjectionMatrix, m_GL_Camera, m_GL_Light, m_GL_Material);
				m_Pieces[i].m_GL_Position = m_Squares[file * 8 + rank].m_GL_Position;
				m_Squares[file * 8 + rank].m_Piece = &m_Pieces[i];
				file++;
				i++;
			}
		}
	}
}
