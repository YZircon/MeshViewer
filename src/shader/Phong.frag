#version 330 core

in vec3 Color;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Texture{
    sampler2D texture;
    bool activate;
};

uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 EyePos;
uniform Material material;
uniform Texture ambientTexture; // 纹理类型
uniform Texture diffuseTexture;
uniform Texture specularTexture;

void main(){
    float ambientStrength = 0.1;
    vec3 La = LightColor * ambientStrength; // Ambient

    vec3 norm = normalize(Normal);
    vec3 LightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, LightDir), 0.0f);
    vec3 Ld = LightColor * (diff * material.diffuse); // Diffuse

    vec3 ViewDir = normalize(EyePos - FragPos);
    vec3 ReflectDir = reflect(-LightDir, norm);
    float spec = pow(max(0.0f, dot(ViewDir, ReflectDir)), material.shininess);
    vec3 Ls = LightColor * (spec * material.specular); // Specular

    vec3 ambientColor, diffuseColor, specularColor;

    if(ambientTexture.activate == true){
        ambientColor = texture(ambientTexture.texture, TexCoord).rgb;
    }else ambientColor = vec3(1.0, 1.0, 1.0);
    if(diffuseTexture.activate == true){
        diffuseColor = texture(diffuseTexture.texture, TexCoord).rgb;
    }else diffuseColor = vec3(1.0, 1.0, 1.0);
    if(specularTexture.activate == true){
        specularColor = texture(specularTexture.texture, TexCoord).rgb;
    }else specularColor = vec3(1.0, 1.0, 1.0);

    FragColor = vec4((La * ambientColor + Ld * diffuseColor + Ls * specularColor), 1.0);
}