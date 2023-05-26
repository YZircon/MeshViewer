//
// Created by yizr_cnyali on 2023/5/7.
//

#ifndef GLYT_MATERIAL_H
#define GLYT_MATERIAL_H
#include <Eigen/Dense>
#include <string>

class Texture2D{
public:
    Texture2D(){}
    Texture2D(std :: string& dir);
    ~Texture2D();
protected:

};

class Material{
public:
    Material(std :: string name,
             Eigen :: Vector3f ambient,
             Eigen :: Vector3f diffuse,
             Eigen :: Vector3f specular,
             float specluar_exponent,
             float optical_density,
             float dissolve,
             int illumination,
             std :: string AmbientTexPath,
             std :: string DiffuseTexPath,
             std :: string SpecluarTexPath,
             std :: string AlphaTexPath);
    ~Material();
protected:
    std :: string Name;

    Eigen :: Vector3f Ambient;
    Eigen :: Vector3f Diffuse;
    Eigen :: Vector3f Specular;

    float SpecluarExponent;
    float OpticalDensity;
    float Dissolve;
    int Illumination;

    Texture2D AmbientTexture;
    Texture2D DiffuseTexture;
    Texture2D SpecluarTexture;
    Texture2D AlphaTexture;
};


#endif //GLYT_MATERIAL_H
