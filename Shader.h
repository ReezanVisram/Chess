#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
	unsigned int id;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	Shader() = default;

	void use();

	void setBoolUniform(const std::string& name, bool value) const;
	void setIntUniform(const std::string& name, int value) const;
	void setFloatUniform(const std::string& name, float value) const;
	void setMat4Uniform(const std::string& name, glm::mat4 value) const;
	void setVec3Uniform(const std::string& name, glm::vec3 value) const;
};

#endif