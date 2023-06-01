//
// Created by yizr_cnyali on 2023/5/4.
//

#ifndef GLYT_CAMERA_H
#define GLYT_CAMERA_H

#include <Eigen/Dense>
#include "Geometry.h"

class Camera {
public:
    Camera(
            Eigen :: Vector3f position,
            Eigen :: Vector3f direction,
            Eigen :: Vector3f worldup,
            float movespeed,
            float eyefov,
            float aspectratio
    );
    ~Camera();

    Eigen :: Vector3f GetPosition() const{ return Position; }

    Eigen :: Vector3f GetDirection() const{ return Direction; }

    Eigen :: Vector3f GetWorldUpDirection() const{ return WorldUp; }

    float GetMoveSpeed() const{ return MoveSpeed; }

    float GetEyeFov() const{ return EyeFov; }

    float GetAspectRatio() const{ return AspectRatio; }

    Eigen :: Matrix4f GetViewMatrix() const { return get_view_matrix(Position, Position + Direction, WorldUp); };

    virtual Eigen :: Matrix4f GetProjectionMatrix() const {return Eigen :: Matrix4f :: Identity();}

    void SetPosition(Eigen :: Vector3f Pos){ Position = Pos; }

    void SetDirection(Eigen :: Vector3f Dir){ Direction = Dir; }

    void SetWorldUpDirection(Eigen :: Vector3f WorldUpDir){ WorldUp = WorldUpDir; }

    void SetMoveSpeed(float speed){ MoveSpeed = speed; }

    void SetEyeFov(float fov){ EyeFov = fov; }

    void SetAspectRatio(float aspect_ratio){ AspectRatio = aspect_ratio; }

    void handleKeyboardInput(int key, float deltatime); /*TODO: Handle Keyboard Input*/

    void handleScrollInput(); /*TODO: Handle Scorll Input*/

    void handleMouseInput(); /*TODO: Handle Mouse Input*/

protected:
    Eigen :: Vector3f Position = Eigen :: Vector3f(0.0f, 0.0f, 0.0f); // Camera's Position
    Eigen :: Vector3f Direction = Eigen :: Vector3f(0.0f, 0.0f, -1.0f); // Camera Look at this direction
    Eigen :: Vector3f WorldUp = Eigen :: Vector3f(0.0f, 1.0f, 0.0f); // Up direction of the world

    float MoveSpeed = 2.5f;
    float EyeFov;
    float AspectRatio;
};

class PerspectiveCamera : public Camera{
public:
    PerspectiveCamera(
            Eigen :: Vector3f position,
            Eigen :: Vector3f direction,
            Eigen :: Vector3f worldup,
            float movespeed,
            float eyefov,
            float aspectratio,
            float znear,
            float zfar
    ):Camera(position, direction, worldup, movespeed, eyefov, aspectratio){
        zFar = zfar;
        zNear = znear;
    };

    float GetzFar() const { return zFar; }

    float GetzNear() const { return zNear; }

    Eigen :: Vector3f GetPos() const{ return Position; };

    Eigen :: Matrix4f GetProjectionMatrix() const override{ return get_projection_matrix(EyeFov, AspectRatio, zNear, zFar); }

    Eigen :: Matrix4f GetViewMatrix() const { return Camera :: GetViewMatrix(); }

    void handleKeyboardInput(int key, float deltatime){
        Camera :: handleKeyboardInput(key, deltatime);
    }

    void SetzFar(float far) { zFar = far; }

    void SetzNear(float near) { zNear = near; }

protected:
    float zFar;
    float zNear;
};

#endif //MESHVIEWER_CAMERA_H
