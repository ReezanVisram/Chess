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
}

void Board::Draw(glm::vec3 mouseRay) {
	for (int i = 0; i < 64; i++) {
		m_Squares[i].Draw();
	}
}
