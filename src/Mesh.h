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
    std :: unordered_map <std :: string, Texture2D *> textures;
    std :: unordered_map <std :: string, Material *> materials;
    /*TODO: 增加材质,所有的材质都在 Model中实例化,但给 Mesh 用指针传进去,对于每个材质,我们使用名字作为唯一标识,用unordered_map找此前是否已经加载该材质*/
};


#endif //GLYT_MESH_H
