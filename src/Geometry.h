//
// Created by yizr_cnyali on 2023/5/4.
//


#ifndef GLYT_GEOMETRY_H
#define GLYT_GEOMETRY_H

#include <Eigen/Dense>

Eigen::Matrix4f get_model_matrix(float angle, Eigen :: Vector3f Scale);
Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos, Eigen::Vector3f target, Eigen::Vector3f Up);//相机位置,相机注视的点,世界空间中的上方
Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);

#endif //GLYT_GEOMETRY_H
