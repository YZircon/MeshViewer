#version 330

layout (location = 0) in vec3 aPosition; // 点的初始位置, 用 Model矩阵把它移动到世界坐标中实际的位置, 然后根据相机进行投影
layout (location = 1) in vec3 aColor; // 实际的颜色,需要经过处理得到看到的颜色

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Color; //处理后的颜色, 处理后的位置直接写道gl_Position里了

void main() {
    gl_Position = Projection * View * Model * vec4(aPosition, 1.0);
    //Color = aColor;
    Color = vec3(1.0f,1.0f,1.0f);
}
