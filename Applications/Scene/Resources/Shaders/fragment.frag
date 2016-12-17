#version 430 core

in vec2 outTextureCoord;
out vec4 color;

uniform sampler2D textureSampler;

layout(std140) uniform Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shinines;	
};

void main(void)
{
    vec3 combinedColor = ambient + diffuse + (specular * 0) + (shinines * 0);
    color = vec4(combinedColor ,1.0f);
}