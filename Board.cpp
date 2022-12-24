#include <stb_image.h>
#include <map>

#include "Board.h"
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"

Board::Board(const char* darkSquareTexturePath, const char* lightSquareTexturePath, float boardAngle, glm::vec3 startingPoint, const char* renderVertexShaderPath, const char* renderFragmentShaderPath, Camera camera, Light light, Material material) {
	this->viewMatrix = camera.getViewMatrix();
	this->projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
	this->darkSquareTexturePath = darkSquareTexturePath;
	this->lightSquareTexturePath = lightSquareTexturePath;
	this->boardAngle = boardAngle;
	this->startingPoint = startingPoint;
	this->renderShader = Shader(renderVertexShaderPath, renderFragmentShaderPath);
	this->camera = camera;
	this->light = light;
	this->material = material;

	glGenTextures(1, &darkTexture);
	glGenTextures(1, &lightTexture);


	glm::vec3 darkVector = startingPoint;
	glm::vec3 lightVector = glm::vec3(startingPoint.x, startingPoint.y + glm::cos(glm::radians(boardAngle)), startingPoint.z - glm::sin(boardAngle));
	
	// Initialize dark squares
	for (int i = 0; i < 32; i++) {
		darkSquares[i] = darkVector;
		darkVector.y += 2 * glm::cos(glm::radians(boardAngle));
		darkVector.z -= 2 * glm::sin(glm::radians(boardAngle));

		if (i % 8 == 3) {
			++darkVector.x;
			darkVector.y = startingPoint.y + glm::cos(glm::radians(boardAngle));
			darkVector.z = startingPoint.z - glm::sin(glm::radians(boardAngle));
		}
		else if (i % 8 == 7) {
			++darkVector.x;
			darkVector.y = startingPoint.y;
			darkVector.z = startingPoint.z;
		}
	}

	// Initialize light squares
	for (int i = 0; i < 32; i++) {
		lightSquares[i] = lightVector;
		lightVector.y += 2 * glm::cos(glm::radians(boardAngle));
		lightVector.z -= 2 * glm::sin(glm::radians(boardAngle));

		if (i % 8 == 3) {
			++lightVector.x;
			lightVector.y = startingPoint.y;
			lightVector.z = startingPoint.z;
		}
		else if (i % 8 == 7) {
			++lightVector.x;
			lightVector.y = startingPoint.y + glm::cos(glm::radians(boardAngle));
			lightVector.z = startingPoint.z - glm::sin(glm::radians(boardAngle));
		}
	}

	initializeBoard();
	
	std::map<int, char> indexToFile = { {0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'} };

	for (int i = 0; i < 8; i++) {
		pieces[i] = Pawn(indexToFile[i], '2', "Assets/Chess/Light/Pawn/Pawn.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	}


	pieces[8] = Rook('a', '1', "Assets/Chess/Light/Rook/Rook.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	pieces[9] = Rook('h', '1', "Assets/Chess/Light/Rook/Rook.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	pieces[10] = Knight('b', '1', "Assets/Chess/Light/Knight/Knight.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	pieces[11] = Knight('g', '1', "Assets/Chess/Light/Knight/Knight.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	pieces[12] = Bishop('c', '1', "Assets/Chess/Light/Bishop/Bishop.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	pieces[13] = Bishop('f', '1', "Assets/Chess/Light/Bishop/Bishop.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	pieces[14] = Queen('d', '1', "Assets/Chess/Light/Queen/Queen.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
	pieces[15] = Bishop('e', '1', "Assets/Chess/Light/King/King.obj", "./Shaders/chesset.vert", "./Shaders/chesset.frag", camera, light, material);
}

void Board::Draw(glm::vec3 mouseRay) {
	renderShader.use();
	renderShader.setMat4Uniform("view", viewMatrix);
	renderShader.setMat4Uniform("projection", projectionMatrix);
	renderShader.setVec3Uniform("viewPos", camera.position);
	renderShader.setVec3Uniform("material.ambient", material.ambient);
	renderShader.setVec3Uniform("material.diffuse", material.diffuse);
	renderShader.setVec3Uniform("material.specular", material.specular);
	renderShader.setFloatUniform("material.shininess", 4.0f);
	renderShader.setVec3Uniform("light.position", light.position);
	renderShader.setVec3Uniform("light.ambient", light.ambient);
	renderShader.setVec3Uniform("light.diffuse", light.diffuse);
	renderShader.setVec3Uniform("light.specular", light.specular);

	// Dark squares
	for (unsigned int i = 0; i < 32; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, darkSquares[i]);
		model = glm::rotate(model, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		renderShader.setMat4Uniform("model", model);
		glBindTexture(GL_TEXTURE_2D, darkTexture);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Light squares
	for (unsigned int i = 0; i < 32; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightSquares[i]);
		model = glm::rotate(model, glm::radians(-boardAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		renderShader.setMat4Uniform("model", model);
		glBindTexture(GL_TEXTURE_2D, lightTexture);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Pieces
	for (int i = 0; i < 16; i++) {
		pieces[i].Draw(mouseRay);
	}
}

void Board::initializeBoard() {
	float squareVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.15f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.15f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.15f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.15f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.15f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.15f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.15f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.15f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.15f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.15f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.15f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.15f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.15f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.15f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.15f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.15f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.15f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.15f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.15f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.15f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.15f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.15f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.15f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.15f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.15f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// Start with dark square texture
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

	// Now configure light square texture
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

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}