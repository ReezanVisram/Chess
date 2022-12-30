#include "Square.h"

Square::Square(const char* darkTexturePath, const char* lightTexturePath, const char* vertexShaderPath, const char* fragmentShaderPath, unsigned int file, unsigned int rank, bool isLight, glm::vec3 position, glm::mat4* projectionMatrix, Camera* camera, Light* light, Material* material) {
	m_File = file;
	m_Rank = rank;

	m_GL_IsLight = isLight;
	m_GL_Position = position;
	m_GL_DarkTexturePath = darkTexturePath;
	m_GL_LightTexturePath = lightTexturePath;
	m_GL_Shader = Shader(vertexShaderPath, fragmentShaderPath);
	m_GL_ProjectionMatrix = projectionMatrix;
	m_GL_Camera = camera;
	m_GL_Light = light;
	m_GL_Material = material;
	m_GL_ViewMatrix = m_GL_Camera->getViewMatrix();
	m_GL_IsSelected = false;
	m_IsLegal = false;

	glGenTextures(1, &m_GL_DarkTexture);
	glGenTextures(1, &m_GL_LightTexture);

	prepareDrawing();
}

void Square::Draw(glm::vec3 mouseRay, bool mouseIsDown) {
	m_GL_Shader.use();
	m_GL_Shader.setMat4Uniform("view", m_GL_ViewMatrix);
	m_GL_Shader.setMat4Uniform("projection", *m_GL_ProjectionMatrix);
	m_GL_Shader.setVec3Uniform("viewPos", m_GL_Camera->position);
	m_GL_Shader.setVec3Uniform("material.ambient", m_GL_Material->ambient);
	m_GL_Shader.setVec3Uniform("material.diffuse", m_GL_Material->diffuse);
	m_GL_Shader.setVec3Uniform("material.specular", m_GL_Material->specular);
	m_GL_Shader.setFloatUniform("material.shininess", m_GL_Material->shininess);
	m_GL_Shader.setVec3Uniform("light.position", m_GL_Light->position);
	m_GL_Shader.setVec3Uniform("light.ambient", m_GL_Light->ambient);
	m_GL_Shader.setVec3Uniform("light.diffuse", m_GL_Light->diffuse);
	m_GL_Shader.setVec3Uniform("light.specular", m_GL_Light->specular);
	m_GL_Shader.setBoolUniform("isSelected", m_GL_IsSelected);
	m_GL_Shader.setBoolUniform("isLegal", m_IsLegal);

	m_GL_ModelMatrix = glm::mat4(1.0f);
	m_GL_ModelMatrix = glm::translate(m_GL_ModelMatrix, m_GL_Position);
	m_GL_Shader.setMat4Uniform("model", m_GL_ModelMatrix);
	if (m_GL_IsLight) {
		glBindTexture(GL_TEXTURE_2D, m_GL_LightTexture);
		glBindVertexArray(m_GL_Vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, m_GL_DarkTexture);
		glBindVertexArray(m_GL_Vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	if (m_Piece != nullptr) {
		m_Piece->Draw(mouseRay, m_GL_Position);
	}
}

bool Square::IsSelected(glm::vec3 mouseRay) {
	float minIntersectionDistance = 0.0f;
	float maxIntersectionDistance = 100000.0f;

	glm::vec3 boundingBoxPositionWorldSpace = glm::vec3(m_GL_ModelMatrix[3].x, m_GL_ModelMatrix[3].y, m_GL_ModelMatrix[3].z);
	glm::vec3 distanceToCamera = boundingBoxPositionWorldSpace - m_GL_Camera->position;

	glm::vec3 aabbMin = glm::vec3(-0.5f, -0.5f, -0.15f);
	glm::vec3 aabbMax = glm::vec3(0.5f, 0.5f, 0.15f);

	glm::vec3 xAxis = glm::vec3(m_GL_ModelMatrix[0].x, m_GL_ModelMatrix[0].y, m_GL_ModelMatrix[0].z);
	float xAxisE = glm::dot(xAxis, distanceToCamera);
	float xAxisF = glm::dot(mouseRay, xAxis);

	if (std::fabs(xAxisF) > 0.0001f) {
		float intersection1 = (xAxisE + aabbMin.x) / xAxisF;
		float intersection2 = (xAxisE + aabbMax.x) / xAxisF;

		if (intersection1 > intersection2) {
			// Swap
			float tempIntersection = intersection1;
			intersection1 = intersection2;
			intersection2 = tempIntersection;
		}

		if (intersection2 < maxIntersectionDistance) {
			maxIntersectionDistance = intersection2;
		}

		if (intersection1 > minIntersectionDistance) {
			minIntersectionDistance = intersection1;
		}

		if (maxIntersectionDistance < minIntersectionDistance) {
			return false;
		}
	}
	else {
		if (-xAxisE + aabbMin.x > 0.0f || -xAxisE + aabbMax.x < 0.0f) {
			return false;
		}
	}

	glm::vec3 yAxis = glm::vec3(m_GL_ModelMatrix[1].x, m_GL_ModelMatrix[1].y, m_GL_ModelMatrix[1].z);

	float yAxisE = glm::dot(yAxis, distanceToCamera);
	float yAxisF = glm::dot(mouseRay, yAxis);

	if (std::fabs(yAxisF) > 0.0001f) {
		float intersection1 = (yAxisE + aabbMin.y) / yAxisF;
		float intersection2 = (yAxisE + aabbMax.y) / yAxisF;

		if (intersection1 > intersection2) {
			float tempIntersection = intersection1;
			intersection1 = intersection2;
			intersection2 = tempIntersection;
		}

		if (intersection2 < maxIntersectionDistance) {
			maxIntersectionDistance = intersection2;
		}

		if (intersection1 > minIntersectionDistance) {
			minIntersectionDistance = intersection1;
		}

		if (maxIntersectionDistance < minIntersectionDistance) {
			return false;
		}
	}
	else {
		if (-yAxisE + aabbMin.y > 0.0f || -yAxisE + aabbMax.y < 0.0f) {
			return false;
		}
	}

	glm::vec3 zAxis = glm::vec3(m_GL_ModelMatrix[2].x, m_GL_ModelMatrix[2].y, m_GL_ModelMatrix[2].z);

	float zAxisE = glm::dot(zAxis, distanceToCamera);
	float zAxisF = glm::dot(mouseRay, zAxis);

	if (std::fabs(zAxisF) > 0.0001f) {
		float intersection1 = (zAxisE + aabbMin.z) / zAxisF;
		float intersection2 = (zAxisE + aabbMax.z) / zAxisF;

		if (intersection1 > intersection2) {
			float tempIntersection = intersection1;
			intersection1 = intersection2;
			intersection2 = tempIntersection;
		}

		if (intersection2 < maxIntersectionDistance) {
			maxIntersectionDistance = intersection2;
		}

		if (intersection1 > minIntersectionDistance) {
			minIntersectionDistance = intersection1;
		}

		if (maxIntersectionDistance < minIntersectionDistance) {
			return false;
		}
	}
	else {
		if (-zAxisE + aabbMin.z > 0.0f || -zAxisE + aabbMax.z < 0.0f) {
			return false;
		}
	}

	return true;
}

void Square::prepareDrawing() {
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
	glBindTexture(GL_TEXTURE_2D, m_GL_DarkTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int darkWidth, darkHeight, darkNrChannels;
	unsigned char* darkData = stbi_load(m_GL_DarkTexturePath, &darkWidth, &darkHeight, &darkNrChannels, 0);
	if (darkData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, darkWidth, darkHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, darkData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "ERROR Failed to load Dark Texture" << std::endl;
	}
	stbi_image_free(darkData);

	// Now configure light square texture
	glBindTexture(GL_TEXTURE_2D, m_GL_LightTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int lightWidth, lightHeight, lightNrChannels;
	unsigned char* lightData = stbi_load(m_GL_LightTexturePath, &lightWidth, &lightHeight, &lightNrChannels, 0);
	if (lightData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lightWidth, lightHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, lightData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "ERROR Failed to load Light Texture" << std::endl;
	}
	stbi_image_free(lightData);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &m_GL_Vao);
	glGenBuffers(1, &m_GL_Vbo);

	glBindVertexArray(m_GL_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_GL_Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}