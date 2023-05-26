#version 330 core

in vec3 Color;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 EyePos;

void main(){
    vec3 norm = normalize(Normal);
    vec3 LightDir = normalize(LightPos - FragPos);
    vec3 ViewDir = normalize(EyePos - FragPos);
    vec3 ReflectDir = reflect(-LightDir, norm);

    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    vec3 La = ambientStrength * LightColor;
    vec3 Ld = Color * max(0.f, dot(norm, LightDir));
    vec3 Ls = specularStrength * pow(max(0.0f, dot(ViewDir, ReflectDir)), 32)* Color;

    FragColor = vec4(Color * (La + Ld + Ls), 1.0);
}