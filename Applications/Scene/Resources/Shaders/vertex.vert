#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;

out vec2 outTextureCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    outTextureCoord = textureCoord;
}