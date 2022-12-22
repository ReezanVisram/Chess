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
#include "Light.h"
#include "Piece.h"
#include "Board.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
void process_input(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Chess", NULL, NULL);

	if (!window) {
		std::cerr << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	glViewport(0, 0, 1280, 720);

	Material wood = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.5, 0.5f),
		4.0
	};

	Camera camera(glm::vec3(0.0f, -10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 50.0f);
	Light light = Light(glm::vec3(2.0f, 4.0f, 4.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f));
	Board board("./Textures/dark.jpg", "./Textures/light.jpg", 0.0f, glm::vec3(-4.0f, -3.0f, 0.0f), "./Shaders/square.vert", "./Shaders/square.frag", "./Shaders/picking.vert", "./Shaders/picking.frag", camera, light, wood);

	// Create the framebuffer
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create the picking texture
	unsigned int pickingTexture;
	glGenTextures(1, &pickingTexture);
	glBindTexture(GL_TEXTURE_2D, pickingTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, 1280, 720, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingTexture, 0);

	// Create the texture object for the depth buffer
	unsigned int depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader picking("./Shaders/picking.vert", "./Shaders/picking.frag");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.094f, 0.125f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Picking phase
		board.Draw(true);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
	std::cout << "Mouse is at (" << xPos << ", " << yPos << ") in screen coordinates\n";
	glm::vec3 win = glm::vec3(xPos, yPos, 0);
	glm::vec4 viewport = glm::vec4(0, 0, 1280, 720);

	glm::vec3 worldPos = glm::unProject(win, glm::mat4(1.0f), glm::mat4(1.0f), viewport);

	std::cout << "Mouse is at (" << worldPos.x << ", " << worldPos.y << ") in world coordinates" << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}