#ifndef MODEL_H
#define MODEL_H
#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(char* path);
    void Draw(Shader shader);

private:
    std::vector<Mesh>   meshes;
    std::string         directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H