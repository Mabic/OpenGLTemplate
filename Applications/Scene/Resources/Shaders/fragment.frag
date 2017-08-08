#version 430 core

in vec2 outTextureCoord;
out vec4 outColor;

uniform sampler2D textureSampler;

layout(std140) uniform Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shinines;	
};

layout(std140) uniform Light {
    vec4 position;
    vec4 color;
};

void main(void)
{
    //vec4 combinedColor = ambient + diffuse + (specular * 0) + (shinines * 0);
    //vec4 calPost = color * position * 0;
    //outColor = position + calPost * combinedColor * 0;
	outColor = texture2D(textureSampler, outTextureCoord);
}