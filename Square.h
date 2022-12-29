#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Piece.h"

class Square {
public:
	unsigned int m_File;
	unsigned int m_Rank;
	Piece *m_Piece = nullptr;

	// OpenGL Information
	bool m_GL_IsLight;
	glm::vec3 m_GL_Position;
	const char* m_GL_DarkTexturePath;
	const char* m_GL_LightTexturePath;
	Shader m_GL_Shader;
	unsigned int m_GL_DarkTexture;
	unsigned int m_GL_LightTexture;
	unsigned int m_GL_Vao;
	unsigned int m_GL_Vbo;
	glm::mat4* m_GL_ProjectionMatrix;
	glm::mat4 m_GL_ViewMatrix;
	glm::mat4 m_GL_ModelMatrix;
	Camera* m_GL_Camera;
	Light* m_GL_Light;
	Material* m_GL_Material;
	bool m_GL_IsSelected;

	Square() = default;
	Square(const char* darkTexturePath, const char* lightTexturePath, const char* vertexShaderPath, const char* fragmentShaderPath, unsigned int file, unsigned int rank, bool isLight, glm::vec3 position, glm::mat4* projectionMatrix, Camera *camera, Light *light, Material *material);

	void Draw(glm::vec3 mouseRay, bool mouseIsDown);
	bool IsSelected(glm::vec3 mouseRay);

private:
	void prepareDrawing();
};

#endif