#ifndef PIECE_H
#define PIECE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"

enum Type {
	Rook,
	Knight,
	Bishop,
	Queen,
	King,
	Pawn
};

enum Color {
	Black,
	White
};

class Piece {
public:
	// Piece Information
	Type m_Type;
	Color m_Color;

	// OpenGL Information
	Model m_GL_Model;
	glm::vec3 m_Position;
	bool m_GL_isSelected;
	float m_GL_HitboxRadius = 0.25f;
	glm::mat4 m_GL_ModelMatrix;
	glm::mat4 m_GL_ViewMatrix;
	glm::mat4* m_GL_ProjectionMatrix;
	Camera* m_GL_Camera;
	Light* m_GL_Light;
	Material* m_GL_Material;
	Shader m_GL_Shader;

	Piece(const char* vertexShaderPath, const char* fragmentShaderPath, Color color, Type type, glm::mat4 *projectionMatrix, Camera* camera, Light* light, Material* material);
	Piece() = default;
	void Draw(glm::vec3 position, glm::vec3 mouseRay);

private:
	Model loadModel();
	bool isIntersecting(glm::mat4 modelMatrix, glm::vec3 mouseRay);
};


#endif
