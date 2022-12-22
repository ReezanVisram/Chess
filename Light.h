#ifndef LIGHT_H
#define LIGHT_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(glm::vec3 p, glm::vec3 a, glm::vec3 d, glm::vec3 s);
	Light() = default;
};


#endif
