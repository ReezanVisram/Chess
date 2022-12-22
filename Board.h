#ifndef BOARD_H
#define BOARD_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Piece.h"
#include "Shader.h"
#include "Camera.h"

class Board {
public:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	Piece pieces[32];
	const char* darkSquareTexturePath;
	const char* lightSquareTexturePath;
	float boardAngle;
	glm::vec3 darkSquares[32];
	glm::vec3 lightSquares[32];

	glm::vec3 startingPoint;

	unsigned int darkTexture;
	unsigned int lightTexture;

	Camera camera;
	Shader renderShader;
	Light light;
	Material material;

	unsigned int vao;
	unsigned int vbo;

	Board(const char* darkSquareTexturePath, const char* lightSquareTexturePath, float boardAngle, glm::vec3 startingPoint, const char* renderVertexShaderPath, const char* renderFragmentShaderPath, Camera camera, Light light, Material material);
	void Draw();
private:
	void initializeBoard();
};


#endif
