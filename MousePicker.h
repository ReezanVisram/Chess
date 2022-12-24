#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

class MousePicker {
public:
	glm::vec3 currentRay;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	Camera camera;

	MousePicker(Camera camera, glm::mat4 projectionMatrix) {
		this->camera = camera;
		this->projectionMatrix = projectionMatrix;
		this->viewMatrix = camera.getViewMatrix();
	}

	void update(float mouseX, float mouseY) {
		currentRay = calculateMouseRay(mouseX, mouseY);
	}

private:
	glm::vec3 calculateMouseRay(float mouseX, float mouseY) {
		glm::vec2 normalizedCoords = getNormalizedDeviceCoordinates(mouseX, mouseY);
		glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
		glm::vec4 eyeCoords = toEyeCoords(clipCoords);
		glm::vec3 worldRay = toWorldCoords(eyeCoords);
		return worldRay;
	}

	glm::vec2 getNormalizedDeviceCoordinates(float mouseX, float mouseY) {
		float x = (2.0f * mouseX) / 1280.0f - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / 720.0f;
		return glm::vec2(x, y);
	}

	glm::vec4 toEyeCoords(glm::vec4 clipCoords) {
		glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
		glm::vec4 eyeCoords = invertedProjection * clipCoords;

		return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	}

	glm::vec3 toWorldCoords(glm::vec4 eyeCoords) {
		glm::mat4 invertedView = glm::inverse(viewMatrix);
		glm::vec4 rayWorld = invertedView * eyeCoords;
		glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
		mouseRay = glm::normalize(mouseRay);

		return mouseRay;
	}
};

#endif
