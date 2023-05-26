//
// Created by yizr_cnyali on 2023/5/4.
//

#ifndef GLYT_SHADER_H
#define GLYT_SHADER_H

#include <Eigen/Dense>
#include <string>



class Shader {
public:
    Shader(std :: string vertexShaderPath, std :: string fragShaderPath);
    ~Shader();
    void activate() const;
    void deactivate() const;
    void SetBool(const std :: string &name, bool value) const;
    void SetInt(const std :: string &name, int value) const;
    void SetFloat(const std :: string &name, float value) const;
    void SetVec3(const std :: string &name, Eigen :: Vector3f value) const;
    void SetMat4(const std :: string &name, Eigen :: Matrix4f value) const;
protected:
    unsigned int ID;
};


#endif //GLYT_SHADER_H
