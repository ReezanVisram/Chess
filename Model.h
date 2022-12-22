#ifndef MODEL_H
#define MODEL_H
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

unsigned int TextureFromFile(const char* path, const std::string &directory);

class Model {
public:
	Model(const char* path);
	Model() = default;
	void Draw(Shader &shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadedTextures;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typName);
};

#endif
