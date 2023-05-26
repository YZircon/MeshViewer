//
// Created by yizr_cnyali on 2023/5/26.
//

#ifndef GLYT_LIGHT_H
#define GLYT_LIGHT_H

#include <Eigen/Dense>

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
    Eigen :: Vector3f Position = Eigen :: Vector3f(0.0f, 0.0f, 0.0f); // Camera's Position
    Eigen :: Vector3f Color = Eigen :: Vector3f(0.0f, 0.0f, 0.0f); // Camera Look at this direction

    float Idensity;
};


#endif //GLYT_LIGHT_H
