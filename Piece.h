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

	float leftmostX;
	float rightmostX;
	float hitboxRadius;


	Piece(char file, char rank, const char* modelPath, const char* renderVertexShaderPath, const char* renderFragmentShaderPath, Camera camera, Light light, Material material);
	Piece() = default;
	void Draw(glm::vec3 mouseRay);

	void setObjectId(unsigned int id);
private:
	glm::vec3 squareToVector();
	bool calculateIntersection(glm::vec3 mouseRay);
	void drawBoundingBox();
};


#endif
