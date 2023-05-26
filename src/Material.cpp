//
// Created by yizr_cnyali on 2023/5/7.
//

#include "Material.h"

Texture2D ::Texture2D(std::string &dir) {

}

Texture2D :: ~Texture2D() = default;

Material :: Material(std :: string name,
                    Eigen :: Vector3f ambient,
                    Eigen :: Vector3f diffuse,
                    Eigen :: Vector3f specular,
                    float specluar_exponent,
                    float optical_density,
                    float dissolve,
                    int illumination,
                    std :: string AmbientTexPath = "",
                    std :: string DiffuseTexPath = "",
                    std :: string SpecluarTexPath = "",
                    std :: string AlphaTexPath = ""){
    Name = name;
    Ambient = ambient;
    Diffuse = diffuse;
    Specular = specular;
    SpecluarExponent = specluar_exponent; //高光反射度
    OpticalDensity = optical_density; //折射率
    Dissolve = dissolve; //物体融入背景的效果
    Illumination = illumination; //描述光照模型
    if(!AmbientTexPath.empty()) AmbientTexture = Texture2D(AmbientTexPath);
    if(!DiffuseTexPath.empty()) DiffuseTexture = Texture2D(DiffuseTexPath);
    if(!SpecluarTexPath.empty()) SpecluarTexture = Texture2D(SpecluarTexPath);
    if(!AlphaTexPath.empty()) AlphaTexture = Texture2D(AlphaTexPath);
}

Material :: ~Material() = default;