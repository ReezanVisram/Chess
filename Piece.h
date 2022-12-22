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

class Piece {
public:
	unsigned int objectId;
	char file;
	char rank;
	Model model;
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	Camera camera;
	Shader renderShader;
	Light light;
	Material material;

	Piece(char file, char rank, const char* modelPath, const char* renderVertexShaderPath, const char* renderFragmentShaderPath, const char* pickingVertexShaderPath, const char* pickingFragmentShaderPath, Camera camera, Light light, Material material);
	Piece() = default;
	void Draw();

	void setObjectId(unsigned int id);
private:
	glm::vec3 squareToVector();
};


#endif
