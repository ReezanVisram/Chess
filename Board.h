#ifndef BOARD_H
#define BOARD_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Piece.h"
#include "Shader.h"
#include "Camera.h"
#include "Square.h"

class Board {
public:
	Square m_Squares[64];

	// OpenGL Information
	glm::mat4 m_GL_ViewMatrix;
	glm::vec3 m_GL_StartingPoint;
	Camera *m_GL_Camera;
	Light *m_GL_Light;
	Material *m_GL_Material;
	glm::mat4* m_GL_ProjectionMatrix;

	std::string m_TexturesDirectory;
	std::string m_ShadersDirectory;

	Board(std::string texturesDirectory, glm::vec3 startingPoint, std::string shadersDirectory, glm::mat4 *projectionMatrix, Camera *camera, Light *light, Material *material);
	Board() = default;
	void Draw(glm::vec3 mouseRay);
};


#endif
