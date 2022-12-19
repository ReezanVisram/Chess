#include "Camera.h"
Camera::Camera(glm::vec3 pos, glm::vec3 u, float y, float p) {
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	position = pos;
	worldUp = u;
	yaw = y;
	pitch = p;
	updateCameraVectors();
}

Camera::Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float y, float p) {
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	position = glm::vec3(xPos, yPos, zPos);
	worldUp = glm::vec3(xUp, yUp, zUp);
	yaw = y;
	pitch = p;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::updateCameraVectors() {
	glm::vec3 f;
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(f);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}