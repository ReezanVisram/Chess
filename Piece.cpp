#include <map>
#include "Piece.h"

Piece::Piece(const char* vertexShaderPath, const char* fragmentShaderPath, Color color, Type type, glm::mat4 *projectionMatrix, Camera* camera, Light* light, Material* material) {
	m_GL_Shader = Shader(vertexShaderPath, fragmentShaderPath);
	m_GL_ViewMatrix = camera->getViewMatrix();
	m_GL_ProjectionMatrix = projectionMatrix;
	m_Color = color;
	m_Type = type;
	m_GL_Model = loadModel();
	m_GL_Camera = camera;
	m_GL_Light = light;
	m_GL_Material = material;

}

Model Piece::loadModel() {
	std::string modelLocation = "./Assets/Chess";
	switch (m_Color) {
	case Black:
		modelLocation += "/Black";
		break;
	case White:
		modelLocation += "/White";
		break;
	}

	switch (m_Type) {
	case Bishop:
		modelLocation += "/Bishop/Bishop.obj";
		break;
	case King:
		modelLocation += "/King/King.obj";
		break;
	case Knight:
		modelLocation += "/Knight/Knight.obj";
		break;
	case Pawn:
		modelLocation += "/Pawn/Pawn.obj";
		break;
	case Queen:
		modelLocation += "/Queen/Queen.obj";
		break;
	case Rook:
		modelLocation += "/Rook/Rook.obj";
		break;
	}

	return Model(modelLocation.c_str());
}

void Piece::Draw(glm::vec3 position, glm::vec3 mouseRay) {
	m_GL_Shader.use();
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	m_GL_Shader.setMat4Uniform("model", modelMatrix);
	m_GL_Shader.setMat4Uniform("projection", *m_GL_ProjectionMatrix);
	m_GL_Shader.setMat4Uniform("view", m_GL_ViewMatrix);
	m_GL_Shader.setVec3Uniform("viewPos", m_GL_Camera->position);
	m_GL_Shader.setVec3Uniform("light.position", m_GL_Light->position);
	m_GL_Shader.setVec3Uniform("light.ambient", m_GL_Light->ambient);
	m_GL_Shader.setVec3Uniform("light.diffuse", m_GL_Light->diffuse);
	m_GL_Shader.setVec3Uniform("light.specular", m_GL_Light->specular);
	m_GL_Shader.setVec3Uniform("material.ambient", m_GL_Material->ambient);
	m_GL_Shader.setVec3Uniform("material.diffuse", m_GL_Material->diffuse);
	m_GL_Shader.setVec3Uniform("material.specular", m_GL_Material->specular);
	m_GL_Shader.setFloatUniform("material.shininess", m_GL_Material->shininess);
	m_GL_Shader.setBoolUniform("isIntersecting", isIntersecting(modelMatrix, mouseRay));
	m_GL_Model.Draw(m_GL_Shader);
}

bool Piece::isIntersecting(glm::mat4 modelMatrix, glm::vec3 mouseRay) {
	glm::vec3 center = modelMatrix * glm::vec4((m_GL_Model.minVertexPos.x + m_GL_Model.maxVertexPos.x) / 2, (m_GL_Model.minVertexPos.y + m_GL_Model.maxVertexPos.y) / 2, (m_GL_Model.minVertexPos.z + m_GL_Model.maxVertexPos.z) / 2, 1.0);

	glm::vec3 oc = m_GL_Camera->position - center;
	float b = glm::dot(oc, mouseRay);
	float c = glm::dot(oc, oc) - 0.25f * 0.25f;
	return b * b - c >= 0;
}