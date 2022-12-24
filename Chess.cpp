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
#include "MousePicker.h"

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void process_input(GLFWwindow* window);

Camera camera(glm::vec3(0.0f, -10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 50.0f);
glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
MousePicker mousePicker = MousePicker(camera, projectionMatrix);

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
	glfwSetMouseButtonCallback(window, mouse_button_callback);

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

	
	Light light = Light(glm::vec3(2.0f, 4.0f, 4.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f));
	Board board("./Textures/dark.jpg", "./Textures/light.jpg", 0.0f, glm::vec3(-4.0f, -3.0f, 0.0f), "./Shaders/square.vert", "./Shaders/square.frag", camera, light, wood);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.094f, 0.125f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::vec3 mouseRay = mousePicker.currentRay;
		board.Draw(mouseRay);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		std::cout << "Current mouse ray: (" << mousePicker.currentRay.x << ", " << mousePicker.currentRay.y << ", " << mousePicker.currentRay.z << ").\n";
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
	mousePicker.update((float)xPos, (float)yPos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}