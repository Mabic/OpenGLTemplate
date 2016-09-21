#version 430 core

in vec3 outColor;

out vec4 color;

void main(void)
{
    color = vec4(outColor, 1.0f);
}