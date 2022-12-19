#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	float vertices[] = {
		0.5f, 0.5f,	0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -0.5f, // top right
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -0.5f, // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -0.5f, // bottom left
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -0.5f, // top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	float lampVertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
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

	unsigned int vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int lightVbo, lightVao;
	glGenVertexArrays(1, &lightVao);
	glGenBuffers(1, &lightVbo);
	glBindVertexArray(lightVao);
	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lampVertices), lampVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float boardAngle = 55.0f;

	Shader shader = Shader("./Shaders/plane.vert", "./Shaders/plane.frag");
	Shader lightShader = Shader("./Shaders/lamp.vert", "./Shaders/lamp.frag");

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

	glm::vec3 squareColors[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		shader.use();
		shader.setMat4Uniform("model", model);
		shader.setMat4Uniform("view", view);
		shader.setMat4Uniform("projection", projection);
		shader.setVec3Uniform("lightPos", lampPos);
		shader.setVec3Uniform("viewPos", camera.position);
		for (unsigned int i = 0; i < 32; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, darkSquares[i]);
			model = glm::rotate(model, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			shader.setMat4Uniform("model", model);
			glBindTexture(GL_TEXTURE_2D, darkTexture);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		for (unsigned int i = 0; i < 32; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lightSquares[i]);
			model = glm::rotate(model, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			shader.setMat4Uniform("model", model);
			glBindTexture(GL_TEXTURE_2D, lightTexture);
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		lightShader.use();
		lightShader.setMat4Uniform("model", lampModel);
		lightShader.setMat4Uniform("view", view);
		lightShader.setMat4Uniform("projection", projection);
		glBindVertexArray(lightVao);
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