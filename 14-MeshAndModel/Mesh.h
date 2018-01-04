#ifndef MESH_H
#define MESH_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    GLuint id;
    std::string type;    
};

class Mesh
{
public:
    std::vector<Vertex>         vertices;
    std::vector<GLuint>         indices;
    std::vector<Texture>        textures;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    void Draw(Shader shader);

private:
    GLuint VAO, VBO, EBO;

    void setupMesh();
};
#endif // MESH_H