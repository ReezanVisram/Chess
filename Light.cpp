#include "Light.h"

Light::Light(glm::vec3 p, glm::vec3 a, glm::vec3 d, glm::vec3 s) {
	position = p;
	ambient = a;
	diffuse = d;
	specular = s;
}