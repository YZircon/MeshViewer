#version 330

layout (location = 0) in vec3 aPosition; // 点的初始位置, 用 Model矩阵把它移动到世界坐标中实际的位置, 然后根据相机进行投影
layout (location = 1) in vec3 aNormal; // 法向
layout (location = 2) in vec2 aTexCoord; // 纹理坐标

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 LightSpaceMatrix;

out vec3 Color; //处理后的颜色, 处理后的位置直接写道gl_Position里了
out vec3 FragPos; // 世界坐标中点的位置
out vec3 Normal; // 法向
out vec2 TexCoord;
out vec4 FragPosInLightSpace; //光空间的坐标

void main() {
    gl_Position = Projection * View * Model * vec4(aPosition, 1.0);
    //Color = aColor;
    Color = vec3(1.0f, 1.0f, 1.0f);
    FragPos = vec3(Model * vec4(aPosition, 1.0));
    Normal = aNormal;
    TexCoord = aTexCoord;
    FragPosInLightSpace = LightSpaceMatrix * vec4(FragPos, 1.0);
}
