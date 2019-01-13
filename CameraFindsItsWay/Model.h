#pragma once

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp";
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

#include "Shader.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Model
{
public:
	vector<Texture> textures_loaded;

	Model(const char* path)
	{
		loadModel(path);
	}
	Model() {}
	void Draw(Shader shader);
private:
	vector<Mesh> meshes;
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector <Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};