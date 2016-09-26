#version 430 core

uniform vec3 offset;

in vec3 outColor;

out vec4 color;

void main(void)
{
    color = vec4(outColor, 1.0f) + vec4(offset, 0.0f);
}