//
// Created by yizr_cnyali on 2023/5/5.
//

#include <glad/glad.h>
#include <OBJ_Loader.h>
#include "Mesh.h"
#include <filesystem>
#include <cassert>

Mesh :: Mesh(objl :: Mesh mesh, Material *Material) {
    for(int i = 0; i < mesh.Vertices.size(); i++){
        Eigen :: Vector3f Position = Eigen :: Vector3f(mesh.Vertices[i].Position.X, mesh.Vertices[i].Position.Y, mesh.Vertices[i].Position.Z);
        Eigen :: Vector3f Normal = Eigen :: Vector3f(mesh.Vertices[i].Normal.X, mesh.Vertices[i].Normal.Y, mesh.Vertices[i].Normal.Z);
        Eigen :: Vector2f TexCoords = Eigen :: Vector2f(mesh.Vertices[i].TextureCoordinate.X, mesh.Vertices[i].TextureCoordinate.Y);
        vertices.push_back({Position, Normal, TexCoords});
        std :: cout << "Position: " << Position.x() << ' ' << Position.y() << ' ' << Position.z() << std :: endl;
    }
    for(int i = 0; i < mesh.Indices.size(); i++){
        indices.push_back(mesh.Indices[i]);
    }
    material = Material;
    SetupMesh();
}

Mesh :: ~Mesh() = default;

void Mesh :: SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if(!indices.empty()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    glBindVertexArray(0);
}

void Mesh :: draw(){
    material->activate();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glBindVertexArray(VAO);
    if(!indices.empty()){ // 有indice就根据indice渲染
        glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    }else{ //否则根据顶点渲染
        glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size());
    }
    glBindVertexArray(0);
}

Model :: Model(std :: string dir, Shader *shader) {
    objl :: Loader loader;
    bool loadout = loader.LoadFile(dir);
    if(loadout){
        auto parent_dir = std :: filesystem :: path(dir).parent_path().string();
        for(int i = 0; i < loader.LoadedMaterials.size(); i++){
            objl :: Material curMaterial = loader.LoadedMaterials[i];
            std :: vector <Texture2D *> TexS;
            if(!curMaterial.map_Ka.empty()) TexS.push_back(loadTexture(parent_dir + "/" + curMaterial.map_Ka,"ambient"));
            if(!curMaterial.map_Kd.empty()) TexS.push_back(loadTexture(parent_dir + "/" + curMaterial.map_Kd,"diffuse"));
            if(!curMaterial.map_Ks.empty()) TexS.push_back(loadTexture(parent_dir + "/" + curMaterial.map_Ks,"specular"));
            if(!curMaterial.map_d.empty()) TexS.push_back(loadTexture(parent_dir + "/" + curMaterial.map_d,"alpha"));
            materials[curMaterial.name] = new PhongMaterial(shader,
                                                            TexS,
                                                            Eigen :: Vector3f(curMaterial.Ka.X, curMaterial.Ka.Y, curMaterial.Ka.Z),
                                                            Eigen :: Vector3f(curMaterial.Kd.X, curMaterial.Kd.Y, curMaterial.Kd.Z),
                                                            Eigen :: Vector3f(curMaterial.Ks.X, curMaterial.Ks.Y, curMaterial.Ks.Z),
                                                            curMaterial.Ns);
        }
        for(int i = 0; i < loader.LoadedMeshes.size(); i++){
            objl :: Mesh curMesh = loader.LoadedMeshes[i];
            mesh.push_back(new Mesh(curMesh, materials[curMesh.MeshMaterial.name]));
        }
    }else{
        puts("Fatal Error: Cannot load your .obj file");
    }
    program = shader;
}

void Model :: draw(){
    for(int i = 0; i < mesh.size(); i++){
        mesh[i]->draw();
    }
}

Texture2D *Model :: loadTexture(const std::string &dir, std :: string Type = "none") {
    if(textures.contains(dir)){
        textures[dir]->SetType(Type);
        return textures[dir];
    }
    return textures[dir] = new Texture2D(dir, std :: vector<std :: string> {Type});
}