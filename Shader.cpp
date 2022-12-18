#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string vertexShaderSourceCode;
	std::string fragmentShaderSourceCode;

	std::ifstream vertexShaderSourceFile;
	std::ifstream fragmentShaderSourceFile;

	vertexShaderSourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderSourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexShaderSourceFile.open(vertexShaderPath);
		fragmentShaderSourceFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;

		vertexShaderStream << vertexShaderSourceFile.rdbuf();
		fragmentShaderStream << fragmentShaderSourceFile.rdbuf();

		vertexShaderSourceFile.close();
		fragmentShaderSourceFile.close();

		vertexShaderSourceCode = vertexShaderStream.str();
		fragmentShaderSourceCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		// TODO: Add proper logging utility
		std::cout << "ERROR Shader file not successfully read" << std::endl;
	}

	const char* vertexShaderSourceCodeC = vertexShaderSourceCode.c_str();
	const char* fragmentShaderSourceCodeC = fragmentShaderSourceCode.c_str();

	// Compile shaders
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceCodeC, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		// TODO: Add proper logging utility
		std::cout << "ERROR Vertex shader compilation failed\n" << infoLog << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCodeC, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		// TODO: Add proper logging utility
		std::cout << "ERROR Fragment shader compilation failed\n" << infoLog << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		// TODO: Add proper logging utility
		std::cout << "ERROR Shader program linking failed\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::use() {
	glUseProgram(id);
}

void Shader::setBoolUniform(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setIntUniform(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloatUniform(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

