#include <map>
#include "Piece.h"

Piece::Piece(char file, char rank, const char* modelPath, const char* renderVertexShaderPath, const char* renderFragmentShaderPath, const char* pickingVertexShaderPath, const char* pickingFragmentShaderPath, Camera camera, Light light, Material material) {
	this->file = file;
	this->rank = rank;
	this->model = Model(modelPath);
	this->modelMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::translate(this->modelMatrix, squareToVector());
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	this->viewMatrix = camera.getViewMatrix();
	this->projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
	this->renderShader = Shader(renderVertexShaderPath, renderFragmentShaderPath);
	this->light = light;
	this->material = material;
	this->camera = camera;
}

void Piece::Draw() {
	renderShader.use();
	renderShader.setMat4Uniform("model", modelMatrix);
	renderShader.setMat4Uniform("projection", projectionMatrix);
	renderShader.setMat4Uniform("view", viewMatrix);
	renderShader.setVec3Uniform("viewPos", camera.position);
	renderShader.setVec3Uniform("light.position", light.position);
	renderShader.setVec3Uniform("light.ambient", light.ambient);
	renderShader.setVec3Uniform("light.diffuse", light.diffuse);
	renderShader.setVec3Uniform("light.specular", light.specular);
	renderShader.setVec3Uniform("material.ambient", material.ambient);
	renderShader.setVec3Uniform("material.diffuse", material.diffuse);
	renderShader.setVec3Uniform("material.specular", material.specular);
	renderShader.setFloatUniform("material.shininess", material.shininess);

	model.Draw(renderShader);
}

void Piece::setObjectId(unsigned int id) {
	objectId = id;
}

glm::vec3 Piece::squareToVector() {
	std::map<char, float> files = { {'a', -4.0f}, {'b', -3.0f}, {'c', -2.0f}, {'d', -1.0f}, {'e', 0.0f}, {'f', 1.0f}, {'g', 2.0f}, {'h', 3.0f} };
	std::map<char, float> ranks = { {'1', -3.0f}, {'2', -2.0f}, {'3', -1.0f}, {'4', 0.0f}, {'5', 1.0f}, {'6', 2.0f}, {'7', 3.0f}, {'8', 4.0f} };

	return glm::vec3(files[file], ranks[rank], 0.0f);
}