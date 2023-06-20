//
// Created by yizr_cnyali on 2023/5/26.
//

#ifndef GLYT_LIGHT_H
#define GLYT_LIGHT_H

#include <Eigen/Dense>
#include "Geometry.h"

class Light {
public:
    Light(
            Eigen :: Vector3f position,
            Eigen :: Vector3f color,
            float idensity
    );
    ~Light();

    Eigen :: Vector3f GetPosition() const{ return Position; }

    Eigen :: Vector3f GetColor() const{ return Color; }

    float GetIdensity() const{ return Idensity; }

    void SetPosition(Eigen :: Vector3f Pos){ Position = Pos; }

    void SetColor(Eigen :: Vector3f Col){ Color = Col; }

    void SetIdensity(float idensity){ Idensity = idensity; }

protected:
    Eigen :: Vector3f Position = Eigen :: Vector3f(0.0f, 0.0f, 0.0f); // Light's Position
    Eigen :: Vector3f Color = Eigen :: Vector3f(0.0f, 0.0f, 0.0f); // Light's Color

    float Idensity;
};

class DirectedLight : public Light{
public:
    DirectedLight(
            Eigen :: Vector3f position,
            Eigen :: Vector3f color,
            float idensity,
            Eigen :: Vector3f direction
    ):Light(position, color, idensity){
        Direction = direction;
    };

    Eigen :: Matrix4f GetViewMatrix() const { return get_view_matrix(Position, Position + Direction, WorldUp); };

    Eigen :: Matrix4f GetProjectionMatrix() const { return get_ortho_projection_matrix(-10.0f, 10.0f, -10.0f, 10.0f, 0.5f, 7.5f); };

    Eigen :: Vector3f GetPosition() const{ return Light :: GetPosition(); }

    Eigen :: Vector3f GetColor() const{ return Light :: GetColor(); }

    Eigen :: Vector3f GetDirection(){ return Direction; };

    void SetDirection(Eigen :: Vector3f Dir){ Direction = Dir; }

protected:
    Eigen :: Vector3f Direction = Eigen :: Vector3f(0.0f, 0.0f, -1.0f);
    Eigen :: Vector3f WorldUp = Eigen :: Vector3f(0.0f, 1.0f, 0.0f); // Up direction of the world
};


#endif //GLYT_LIGHT_H
