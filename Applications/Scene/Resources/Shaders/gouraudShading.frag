#version 430 core

in vec3 colorPerVertex;
out vec4 outColor;

void main(void)
{
    outColor = vec4(colorPerVertex, 1.0f);
}