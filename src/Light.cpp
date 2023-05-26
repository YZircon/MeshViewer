//
// Created by yizr_cnyali on 2023/5/26.
//

#include "Light.h"

Light :: Light(Eigen :: Vector3f position, Eigen :: Vector3f color, float idensity) {
    Position = position;
    Color = color;
    Idensity = idensity;
}

Light :: ~Light() = default;