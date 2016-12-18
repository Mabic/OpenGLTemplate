#version 430 core

in vec2 outTextureCoord;
out vec4 outColor;

uniform sampler2D textureSampler;

layout(std140) uniform Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shinines;	
};

layout(std140) uniform Light {
    vec4 position;
    vec3 color;
};

void main(void)
{
    vec3 combinedColor = ambient + diffuse + (specular * 0) + (shinines * 0);
    vec4 calPost = vec4(color,1.0f) * position * 0;
    outColor = vec4(combinedColor, 1.0f) + calPost;
}