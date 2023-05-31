//
// Created by yizr_cnyali on 2023/5/7.
//

#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include <iostream>

#include <glad/glad.h>

Texture2D ::Texture2D(const std :: string &dir, std :: vector<std :: string> Types) {
    stbi_set_flip_vertically_on_load(true); // 反转纹理贴图的Y轴
    int channels;
    data = stbi_load(dir.c_str(), &width, &height, &channels, 0);
    if(data == nullptr){
        std :: cout << "The Model have no Texture" << dir << std :: endl;
    }

    types = Types; // 标记纹理类型

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 设置过滤方式

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);//生成多级渐远纹理(glad库)

}

Texture2D :: ~Texture2D(){
    if(data == nullptr) return;
    glDeleteTextures(1, &ID);
    stbi_image_free((void *)data);
}

void Texture2D :: activate(int id) {
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, ID);
}

Material ::Material(Shader *shader, std::vector<Texture2D *> Textures) {
    program = shader;
    textures = Textures;
}

Material :: ~Material() = default;

void Material :: activate() {
    program->activate();
    for(int i = 0; i < textures.size(); i++){
        textures[i]->activate(i);
        std :: vector<std :: string> types = textures[i]->GetType(); // 根据类型手动挂载纹理
        for(auto type : types) {
            if (type == "none") continue;
            if (type == "ambient") {
                glUniform1i(glGetUniformLocation(textures[i]->GetID(), "ambientTexture.texture"), 0);
                program->SetBool("ambientTexture.activate", true);
            }
            if (type == "diffuse") {
                glUniform1i(glGetUniformLocation(textures[i]->GetID(), "diffuseTexture.texture"), 0);
                program->SetBool("diffuseTexture.activate", true);
            }
            if (type == "specular") {
                glUniform1i(glGetUniformLocation(textures[i]->GetID(), "specularTexture.texture"), 0);
                program->SetBool("specularTexture.activate", true);
            }
        }
    }
}

void PhongMaterial :: activate() {
    program->SetVec3("material.ambient", texAmbient);
    program->SetVec3("material.diffuse", texDiffuse);
    program->SetVec3("material.specular", texSpecular);
    program->SetFloat("material.shininess", shininess);
    Material :: activate();
}