//
// Created by yizr_cnyali on 2023/5/7.
//

#ifndef GLYT_MATERIAL_H
#define GLYT_MATERIAL_H
#include <Eigen/Dense>
#include <string>
#include <algorithm>
#include "Shader.h"

class Material;

class Texture2D{
    friend Material;
public:
    Texture2D(){}
    explicit Texture2D(const std :: string& dir, std :: vector<std :: string> Types);
    ~Texture2D();

    virtual void activate(int id);

    std :: vector <std :: string> GetType() const{
        return types;
    }

    void SetType(std :: string Type){
        if(std :: find(types.begin(), types.end(), Type) == types.end()){ // 如果这个纹理未作为Type类型使用,添加
            types.push_back(Type);
        }
    }

    unsigned int GetID() const{
        return ID;
    }
protected:
    const unsigned char *data = nullptr;
    unsigned int ID;
    int width, height;
    std :: vector <std :: string> types; // 一张图可以是多个类型的纹理,纹理类型, ambient = Ambient Texture, diffuse = Diffuse Texture, specular = Specular Texture, alpha = Alpha Texture, bump = Bump map
};

class Material{
public:
    Material(){}
    Material(Shader *shader, std :: vector<Texture2D *> textures);
    ~Material();

    virtual void activate();
protected:
    Shader *program;

    std :: vector <Texture2D *> textures;
};

class PhongMaterial: public Material{
public:
    PhongMaterial(){}

    PhongMaterial(Shader *shader, std :: vector<Texture2D *> textures, Eigen :: Vector3f TexAmbient, Eigen :: Vector3f TexDiffuse, Eigen :: Vector3f TexSpecular, float Shininess) : Material(shader, textures){
        texAmbient = TexAmbient;
        texDiffuse = TexDiffuse;
        texSpecular = TexSpecular;
        shininess = Shininess;
    }

    void activate();

protected:
    Eigen :: Vector3f texDiffuse;
    Eigen :: Vector3f texSpecular;
    Eigen :: Vector3f texAmbient;
    float shininess;
};

#endif //GLYT_MATERIAL_H
