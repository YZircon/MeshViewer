//
// Created by yizr_cnyali on 2023/5/5.
//

#include <glad/glad.h>
#include <OBJ_Loader.h>
#include "Mesh.h"

Mesh :: Mesh(objl :: Mesh mesh) {
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

void Mesh :: draw(Shader &shader){
    shader.activate();
    glBindVertexArray(VAO);
    if(!indices.empty()){ // 有indice就根据indice渲染
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }else{ //否则根据顶点渲染
        glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size());
    }
}

Model :: Model(std :: string dir) {
    objl :: Loader loader;
    bool loadout = loader.LoadFile(dir);
    if(loadout){
        for(int i = 0; i < loader.LoadedMeshes.size(); i++){
            mesh.push_back(Mesh(loader.LoadedMeshes[i]));
        }
    }else{
        puts("Fatal Error: Cannot load your .obj file");
    }
}

void Model :: draw(Shader &shader){
    for(int i = 0; i < mesh.size(); i++){
        mesh[i].draw(shader);
    }
}