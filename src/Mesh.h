//
// Created by yizr_cnyali on 2023/5/5.
//

#ifndef GLYT_MESH_H
#define GLYT_MESH_H

#include <Eigen/Dense>
#include <vector>
#include <string>
#include "Shader.h"
#include "Material.h"

namespace objl {
    class Mesh;

    class Loader;
}

struct Vertex{
    Eigen :: Vector3f Position;
    Eigen :: Vector3f Normal;
    Eigen :: Vector2f TexCoord;
};

class Mesh {
public:
    Mesh(objl :: Mesh mesh, Material *material);
    Mesh(std :: vector <Vertex> vert){vertices = vert,SetupMesh();}
    ~Mesh();
    void SetupMesh();
    void draw();
protected:
    std :: vector <Vertex> vertices;
    std :: vector <unsigned  int> indices;
    unsigned int VAO, VBO, EBO;
    Material *material = nullptr;
};

class Model{
public:
    Model(){}
    Model(std :: string dir, Shader *shader);
    ~Model() = default;
    void draw();
    std :: unordered_map <std :: string, float> GetBoundingBox(){
        return BoundingBox;
    }

    Eigen :: Vector3f GetScale(){
        return Scale;
    }

    void SetScale(Eigen :: Vector3f scale){
        Scale = scale;
    }

    void AddMesh(Mesh *x){ // dangerous! force to add a mesh into the model, not inflect the Bounding Box
        mesh.push_back(x);
    }

protected:
    Texture2D *loadTexture(const std :: string &dir, std :: string Type);
    Shader *program = nullptr;
    std :: vector<Mesh *> mesh;
    std :: unordered_map <std :: string, Texture2D *> textures;
    std :: unordered_map <std :: string, Material *> materials;
    std :: unordered_map <std :: string, float> BoundingBox; // 标签为 x_min, x_max, y_min, y_max, z_min, z_max
    Eigen :: Vector3f Scale = Eigen :: Vector3f(1.0, 1.0, 1.0);
};


#endif //GLYT_MESH_H
