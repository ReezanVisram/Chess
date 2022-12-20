#include <iostream>
#include <fstream>

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
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Chess", NULL, NULL);

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

	stbi_set_flip_vertically_on_load(true);

	glViewport(0, 0, 1280, 720);

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

	Camera camera(glm::vec3(0.0f, -10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 50.0f);

	float boardAngle = 0.0f;

	glm::mat4 boardModel = glm::mat4(1.0f);
	boardModel = glm::rotate(boardModel, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));

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

	glm::mat4 kingModel = glm::mat4(1.0f);
	kingModel = glm::translate(kingModel, glm::vec3(0.0f, -3.0f, 0.0f));
	kingModel = glm::rotate(kingModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	kingModel = glm::scale(kingModel, glm::vec3(0.05f, 0.05f, 0.05f));

	glm::mat4 queenModel = glm::mat4(1.0f);
	queenModel = glm::translate(queenModel, glm::vec3(-1.0f, -3.0f, 0.0f));
	queenModel = glm::rotate(queenModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	queenModel = glm::scale(queenModel, glm::vec3(0.05f, 0.05f, 0.05f));

	glm::mat4 view = camera.getViewMatrix();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

	Model king("Assets/Chess/Light/King/King.obj");
	Model queen("Assets/Chess/Light/Queen/Queen.obj");
	Shader chessShader("./Shaders/chesset.vert", "./Shaders/chesset.frag");
	Shader squareShader = Shader("./Shaders/square.vert", "./Shaders/square.frag");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.094f, 0.125f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		squareShader.use();
		squareShader.setMat4Uniform("model", boardModel);
		squareShader.setMat4Uniform("view", view);
		squareShader.setMat4Uniform("projection", projection);
		squareShader.setVec3Uniform("lightPos", glm::vec3(0.0f, -3.5f, 1.0f));
		squareShader.setVec3Uniform("viewPos", camera.position);
		squareShader.setVec3Uniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		squareShader.setVec3Uniform("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		squareShader.setVec3Uniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		squareShader.setFloatUniform("material.shininess", 4.0f);
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
		chessShader.use();
		chessShader.setMat4Uniform("model", kingModel);
		chessShader.setMat4Uniform("view", view);
		chessShader.setMat4Uniform("projection", projection);
		chessShader.setVec3Uniform("light.position", glm::vec3(0.0f, -4.0f, 1.0f));
		chessShader.setVec3Uniform("light.ambient", glm::vec3(0.6f, 0.6f, 0.6f));
		chessShader.setVec3Uniform("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		chessShader.setVec3Uniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		chessShader.setVec3Uniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		chessShader.setVec3Uniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		chessShader.setVec3Uniform("material.shininess", glm::vec3(32.0f));
		chessShader.setVec3Uniform("viewPos", camera.position);
		king.Draw(chessShader);
		chessShader.setMat4Uniform("model", queenModel);
		queen.Draw(chessShader);
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