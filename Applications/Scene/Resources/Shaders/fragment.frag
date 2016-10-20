#version 430 core

in vec2 outTextureCoord;
out vec4 color;

uniform sampler2D textureSampler;

void main(void)
{
    color = texture(textureSampler, outTextureCoord) * vec4(1.0f, 1.0f, 1.0f ,1.0f);
}