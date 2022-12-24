#include <map>
#include "Piece.h"

Piece::Piece(char file, char rank, const char* modelPath, const char* renderVertexShaderPath, const char* renderFragmentShaderPath, Camera camera, Light light, Material material) {
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
	std::cout << "Piece on " << file << rank << ": ";
	std::cout << "Min vertex pos: (" << model.minVertexPos.x << ", " << model.minVertexPos.y << ", " << model.minVertexPos.z << ")\n";
	std::cout << "Max vertex pos: (" << model.maxVertexPos.x << ", " << model.maxVertexPos.y << ", " << model.maxVertexPos.z << ")\n";
	// Calculate vertex positions in world space
	std::vector<Mesh> meshes = model.getMeshes();
	leftmostX = (modelMatrix * glm::vec4(meshes[0].vertices[0].Position, 2.0)).x;
	rightmostX = (modelMatrix * glm::vec4(meshes[0].vertices[0].Position, 2.0)).x;

	for (int i = 0; i < meshes.size(); i++) {
		std::vector<Vertex> vertices = meshes[i].vertices;
		for (int j = 0; j < vertices.size(); j++) {
			glm::vec4 worldCoords = modelMatrix * glm::vec4(vertices[j].Position, 1.0);
			if (worldCoords.x < leftmostX) {
				leftmostX = worldCoords.x;
			}
			if (worldCoords.x > rightmostX) {
				rightmostX = worldCoords.x;
			}
		}
	}
	std::cout << " Model is between x = " << leftmostX << " and x = " << rightmostX << std::endl;


}

void Piece::Draw(glm::vec3 mouseRay) {
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
	renderShader.setBoolUniform("isIntersecting", calculateIntersection(mouseRay));
	model.Draw(renderShader);
}

glm::vec3 Piece::squareToVector() {
	std::map<char, float> files = { {'a', -4.0f}, {'b', -3.0f}, {'c', -2.0f}, {'d', -1.0f}, {'e', 0.0f}, {'f', 1.0f}, {'g', 2.0f}, {'h', 3.0f} };
	std::map<char, float> ranks = { {'1', -3.0f}, {'2', -2.0f}, {'3', -1.0f}, {'4', 0.0f}, {'5', 1.0f}, {'6', 2.0f}, {'7', 3.0f}, {'8', 4.0f} };

	return glm::vec3(files[file], ranks[rank], 0.0f);
}

bool Piece::calculateIntersection(glm::vec3 mouseRay) {
	glm::vec3 center = modelMatrix * glm::vec4((model.minVertexPos.x + model.maxVertexPos.x) / 2, (model.minVertexPos.y + model.maxVertexPos.y) / 2, (model.minVertexPos.z + model.maxVertexPos.z) / 2, 1.0);

	glm::vec3 oc = camera.position - center;
	float b = glm::dot(oc, mouseRay);
	float c = glm::dot(oc, oc) - 0.25f * 0.25f;
	return b * b - c >= 0;
}
