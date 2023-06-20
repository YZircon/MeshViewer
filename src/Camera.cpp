//
// Created by yizr_cnyali on 2023/5/4.
//

#include <GLFW/glfw3.h>
#include "camera.h"

constexpr double MY_PI = 3.141592653589;

Camera :: Camera(
        Eigen :: Vector3f position,
        Eigen :: Vector3f direction,
        Eigen :: Vector3f worldup,
        float movespeed,
        float eyefov,
        float aspectratio
        ) {
    Position = position;
    Direction = direction;
    WorldUp = worldup;
    MoveSpeed = movespeed;
    EyeFov = eyefov;
    AspectRatio = aspectratio;
}

Camera :: ~Camera() = default;

void Camera :: handleKeyboardInput(int key, float deltatime) {
    float Speed = MoveSpeed * deltatime;
    if(key == GLFW_KEY_W){ //拉近
        Position += Speed * Direction;
    }
    if(key == GLFW_KEY_S){ //远离
        Position -= Speed * Direction;
    }
    if(key == GLFW_KEY_A){ //左移
        Position -= (Direction.cross(WorldUp)).normalized() * Speed;
    }
    if(key == GLFW_KEY_D){ //右移
        Position += (Direction.cross(WorldUp)).normalized() * Speed;
    }
    if(key == GLFW_KEY_Q){ //上移
        Position -= (Direction.cross(Direction.cross(WorldUp))).normalized() * Speed;
    }
    if(key == GLFW_KEY_E){ //下移
        Position += (Direction.cross(Direction.cross(WorldUp))).normalized() * Speed;
    }
}

void Camera :: handleMouseInput(float xoffset, float yoffset) {
    Yaw += xoffset;
    Pitch += yoffset;

    if(Pitch > 89.0f) Pitch = 89.0f;
    if(Pitch < -89.0f) Pitch = -89.0f;

    double yaw = Yaw * MY_PI / 180.f;
    double pitch = Pitch * MY_PI / 180.f;

    Direction = Eigen :: Vector3f(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)).normalized();
}

void Camera :: handleScrollInput() {

}
