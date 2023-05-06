//
// Created by yizr_cnyali on 2023/5/5.
//

#ifndef GLYT_MESH_H
#define GLYT_MESH_H

#include <Eigen/Dense>
#include <vector>
#include <string>
#include "Shader.h"

namespace objl {
    class Mesh;

    class Loader;
}

struct Vertex{
    Eigen :: Vector3f Position;
    Eigen :: Vector3f Normal;
    Eigen :: Vector2f TexCoords;
};

class Mesh {
public:
    Mesh(objl :: Mesh mesh);
    Mesh(std :: vector <Vertex> vert){vertices = vert,SetupMesh();}
    ~Mesh();
    void SetupMesh();
    void draw(Shader &shader);
protected:
    std :: vector <Vertex> vertices;
    std :: vector <unsigned  int> indices;
    unsigned int VAO, VBO, EBO;
};

class Model{
public:
    Model(std :: string dir);
    ~Model() = default;
    void draw(Shader &shader);
protected:
    std :: vector<Mesh> mesh;
};


#endif //GLYT_MESH_H
