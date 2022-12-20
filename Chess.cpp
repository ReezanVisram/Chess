#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Chess", NULL, NULL);

	if (!window) {
		std::cerr << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.15f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.15f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.15f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.15f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.15f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.15f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.15f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.15f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.15f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.15f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.15f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.15f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.15f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.15f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.15f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.15f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.15f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.15f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.15f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.15f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.15f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.15f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	unsigned int darkTexture;
	unsigned int lightTexture;
	glGenTextures(1, &darkTexture);
	glGenTextures(1, &lightTexture);
	glBindTexture(GL_TEXTURE_2D, darkTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int darkWidth, darkHeight, darkNrChannels;
	unsigned char* darkData = stbi_load("./Textures/dark.jpg", &darkWidth, &darkHeight, &darkNrChannels, 0);
	if (darkData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, darkWidth, darkHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, darkData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "ERROR Failed to load Dark Texture" << std::endl;
	}
	stbi_image_free(darkData);

	glBindTexture(GL_TEXTURE_2D, lightTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int lightWidth, lightHeight, lightNrChannels;
	unsigned char* lightData = stbi_load("./Textures/light.jfif", &lightWidth, &lightHeight, &lightNrChannels, 0);
	if (lightData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lightWidth, lightHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, lightData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "ERROR Failed to load Light Texture" << std::endl;
	}
	stbi_image_free(lightData);

	unsigned int borderTexture;
	glGenTextures(1, &borderTexture);
	glBindTexture(GL_TEXTURE_2D, borderTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int borderWidth, borderHeight, borderNrChannels;
	unsigned char* borderData = stbi_load("./Textures/border.jpg", &borderWidth, &borderHeight, &borderNrChannels, 0);
	if (borderData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, borderWidth, borderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, borderData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "ERROR Failed to load Border Texture" << std::endl;
	}
	stbi_image_free(borderData);
	unsigned int squaresVbo, squaresVao;
	glGenVertexArrays(1, &squaresVao);
	glGenBuffers(1, &squaresVbo);

	glBindVertexArray(squaresVao);
	glBindBuffer(GL_ARRAY_BUFFER, squaresVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int lampVbo, lampVao;
	glGenVertexArrays(1, &lampVao);
	glGenBuffers(1, &lampVbo);
	glBindVertexArray(lampVao);
	glBindBuffer(GL_ARRAY_BUFFER, lampVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float boardAngle = 55.0f;

	Shader squareShader = Shader("./Shaders/square.vert", "./Shaders/square.frag");
	Shader lampShader = Shader("./Shaders/lamp.vert", "./Shaders/lamp.frag");

	glm::vec3 lampPos = glm::vec3(1.2f, 2.0f, 1.0f);
	glm::mat4 lampModel = glm::mat4(1.0f);
	lampModel = glm::translate(lampModel, lampPos);
	lampModel = glm::scale(lampModel, glm::vec3(0.2f));

	Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = camera.getViewMatrix();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	glm::vec3 darkSquares[32];
	glm::vec3 lightSquares[32];

	float originalX = -4.0f;
	float x2 = -3.0f;
	float originalY = -3.0f;
	float originalZ = 0.0f;

	float darkX = originalX;
	float darkY = originalY;
	float darkZ = originalZ;

	float lightX = originalX;
	float lightY = originalY + glm::cos(glm::radians(boardAngle));
	float lightZ = originalZ - glm::sin(glm::radians(boardAngle));

	for (int i = 0; i < 32; i++) {
		darkSquares[i] = glm::vec3(darkX, darkY, darkZ);

		darkY += glm::cos(glm::radians(boardAngle));
		darkY += glm::cos(glm::radians(boardAngle));
		darkZ -= glm::sin(glm::radians(boardAngle));
		darkZ -= glm::sin(glm::radians(boardAngle));

		if (i % 8 == 3) {
			darkX += 1;
			darkY = originalY + glm::cos(glm::radians(boardAngle));
			darkZ = originalZ - glm::sin(glm::radians(boardAngle));
		}
		else if (i % 8 == 7) {
			darkX += 1;
			darkY = originalY;
			darkZ = originalZ;
		}
	}

	for (int i = 0; i < 32; i++) {
		lightSquares[i] = glm::vec3(lightX, lightY, lightZ);
		lightY += glm::cos(glm::radians(boardAngle));
		lightY += glm::cos(glm::radians(boardAngle));
		lightZ -= glm::sin(glm::radians(boardAngle));
		lightZ -= glm::sin(glm::radians(boardAngle));

		if (i % 8 == 3) {
			lightX += 1;
			lightY = originalY;
			lightZ = originalZ;
		}
		else if (i % 8 == 7) {
			lightX += 1;
			lightY = originalY + glm::cos(glm::radians(boardAngle));
			lightZ = originalZ - glm::sin(glm::radians(boardAngle));
		}
	}

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		squareShader.use();
		squareShader.setMat4Uniform("model", model);
		squareShader.setMat4Uniform("view", view);
		squareShader.setMat4Uniform("projection", projection);
		squareShader.setVec3Uniform("lightPos", lampPos);
		squareShader.setVec3Uniform("viewPos", camera.position);
		squareShader.setVec3Uniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		squareShader.setVec3Uniform("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		squareShader.setVec3Uniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		squareShader.setFloatUniform("material.shininess", 32.0f);
		squareShader.setVec3Uniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		squareShader.setVec3Uniform("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		squareShader.setVec3Uniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		for (unsigned int i = 0; i < 32; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, darkSquares[i]);
			model = glm::rotate(model, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			squareShader.setMat4Uniform("model", model);
			glBindTexture(GL_TEXTURE_2D, darkTexture);
			glBindVertexArray(squaresVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (unsigned int i = 0; i < 32; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lightSquares[i]);
			model = glm::rotate(model, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			squareShader.setMat4Uniform("model", model);
			glBindTexture(GL_TEXTURE_2D, lightTexture);
			glBindVertexArray(squaresVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		lampShader.use();
		lampShader.setMat4Uniform("model", lampModel);
		lampShader.setMat4Uniform("view", view);
		lampShader.setMat4Uniform("projection", projection);
		glBindVertexArray(lampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}