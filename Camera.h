#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f), float y = YAW, float p = PITCH);
	Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float y, float p);
	glm::mat4 getViewMatrix();

private:
	void updateCameraVectors();
};

#endif