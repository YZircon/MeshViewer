//
// Created by yizr_cnyali on 2023/5/4.
//

#include <glad/glad.h>
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader :: Shader(std :: string vertexShaderPath, std :: string fragmentShaderPath){
    std :: string vertexCode;
    std :: string fragmentCode;
    std :: ifstream vShaderFile;
    std :: ifstream fShaderFile;

    vShaderFile.exceptions(std :: ifstream :: failbit | std :: ifstream :: badbit);
    fShaderFile.exceptions(std :: ifstream :: failbit | std :: ifstream :: badbit);
    try{
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);//打开shader文件
        std :: stringstream vShaderStream,fShaderStream;//读入
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();//关闭文件
        fShaderFile.close();
        vertexCode = vShaderStream.str();//把shader的源码写入string
        fragmentCode = fShaderStream.str();
    }catch(std :: ifstream :: failure e){
        std :: cout << "ERROR : SHADER : FILE_NOT_SUCCESSFULLY_READ" << std :: endl;
    }
    const char* vShaderCode = vertexCode.c_str();//把shader的源码从string写入C风格字符串
    const char* fShaderCode = fragmentCode.c_str();
    unsigned int vertex,fragment;//顶点着色器和片段着色器的ID(OpenGL的一个特征是通过ID引用)
    int success;//着色器编译状态
    char log[512];//存储编译信息

    vertex = glCreateShader(GL_VERTEX_SHADER);//初始化一个顶点着色器
    glShaderSource(vertex,1, &vShaderCode, nullptr);//设置编译顶点着色器需要的信息(要编译的着色器对象,传递的源码字符串数量,源码字符串的引用,NULL)
    glCompileShader(vertex);//进行编译

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, nullptr, log);
        std :: cout << "ERROR : SHADER : PROGRAM : VERTEX_SHADER : COMPILE_ERROR\n" << log << std :: endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);//初始化一个片段着色器
    glShaderSource(fragment,1, &fShaderCode, nullptr);//设置编译片段着色器需要的信息(要编译的着色器对象,传递的源码字符串数量,源码字符串的引用,NULL)
    glCompileShader(fragment);//进行编译

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, nullptr, log);
        std :: cout << "ERROR : SHADER : PROGRAM : FRAGMENT_SHADER : COMPILE_ERROR\n" << log << std :: endl;
    }

    ID = glCreateProgram();//创建着色器程序
    glAttachShader(ID, vertex);//在着色器程序中附加之前编译的顶点和片段着色器
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetShaderiv(fragment, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, nullptr, log);
        std :: cout << "ERROR : SHADER : PROGRAM : LINKING_ERROR\n" << log << std :: endl;
    }

    glDeleteShader(vertex);//着色器已经被链接到程序中了,不再需要了,delete掉
    glDeleteShader(fragment);
}

Shader :: ~Shader() = default;

void Shader :: activate() const{
    glUseProgram(ID);
}

void Shader :: deactivate() const{
    glUseProgram(0);
}

void Shader :: SetBool(const std :: string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader :: SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader :: SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader :: SetVec3(const std::string &name, Eigen :: Vector3f value) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x(), value.y(), value.z());
}

void Shader :: SetMat4(const std::string &name, Eigen::Matrix4f value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value.data()); // Eigen默认是列主序存储,和OpenGL一样,而DirectX是行主序,需要转置;
}