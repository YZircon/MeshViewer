//
// Created by yizr_cnyali on 2023/5/4.
//

#include "Geometry.h"
#include <Eigen/Dense>

constexpr double MY_PI = 3.141592653589;

Eigen::Matrix4f get_model_matrix(float angle) {
    Eigen::Matrix4f rotation;
    angle = angle * MY_PI / 180.f;
    rotation << cos(angle), 0, sin(angle), 0,
            0, 1, 0, 0,
            -sin(angle), 0, cos(angle), 0,
            0, 0, 0, 1;

    Eigen::Matrix4f scale;
    scale << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return translate * rotation * scale;
}

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos, Eigen::Vector3f target, Eigen::Vector3f Up)//相机位置,相机注视的点,世界空间中的上方
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1,0,0,-eye_pos[0],
            0,1,0,-eye_pos[1],
            0,0,1,-eye_pos[2],
            0,0,0,1;

    Eigen::Matrix4f rotate;
    Eigen::Vector3f CameraDir = (eye_pos - target).normalized();
    Eigen::Vector3f CameraRight = (Up.cross(CameraDir)).normalized();
    Eigen::Vector3f CameraUp = (CameraDir.cross(CameraRight)).normalized();
    rotate << CameraRight.x(), CameraRight.y(), CameraRight.z(), 0,
            CameraUp.x(), CameraUp.y(), CameraUp.z(), 0,
            CameraDir.x(), CameraDir.y(), CameraDir.z(), 0,
            0, 0, 0, 1;

    view = rotate * translate * view;

    return view;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    // Create the projection matrix for the given parameters.
    // Then return it.
    //公式中的n代表zNear,f代表zFar
    //如果只进行正交投影,由于eye_fov是45°,x轴上方是视场则是22.5°,因此是看不到点v_1的(夹角是45°),画面中只有线段(v_0,v_2)
    //函数中传入的zNear和zFar是近平面和远平面到xOy平面的距离,但课程中n和f表示的是近平面和远平面的z轴值,因此变换矩阵的推导有修正
    eye_fov = eye_fov * MY_PI / 180.0;//似乎输入的单位是°,但是tanf等函数要求的是弧度
    float t = tanf(eye_fov / 2.0) * fabs(zNear);
    float r = aspect_ratio * t;
    float l = -r,b = -t;
    Eigen :: Matrix4f translate;
    translate << 1, 0, 0, -(r + l) / 2,
            0, 1, 0, -(t + b) / 2,
            0, 0, 1, (zNear + zFar) / 2,
            0, 0, 0, 1;
    Eigen :: Matrix4f scale;
    scale << 2 / (r - l), 0, 0, 0,
            0, 2 / (t - b), 0, 0,
            0, 0, 2 / (zFar - zNear), 0,//缩放倍率显然不是负的,因为zFar和zNear是距离所以改成zFar - zNear
            0, 0, 0, 1;
    Eigen :: Matrix4f ortho = Eigen :: Matrix4f :: Identity();//ortho projection
    ortho = scale * translate * ortho;
    Eigen :: Matrix4f orth2pers;//Orthographic proj to Perspective proj
    orth2pers << zNear, 0, 0, 0,
            0, zNear, 0, 0,
            0, 0, (zNear + zFar), zNear * zFar,
            0, 0, -1, 0;
    //方程应该是 (0,0,A,B) \times (x,y,-zNear,1)的转置 = zNear^2 和 A \times -zFar + B = zFar ^ 2,约束条件约束的是坐标变化,需要带符号
    Eigen :: Matrix4f mirror;//翻转z坐标,因为是右手系
    mirror << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, -1, 0,
            0, 0, 0, 1;
    projection = mirror * ortho * orth2pers * projection;
    return projection;
}