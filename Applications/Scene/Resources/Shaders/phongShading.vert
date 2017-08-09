#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normalPerVertex;
out vec3 vertexPositionInWorldSpace;
out vec2 outTextureCoord;

void main()
{
    // pass normal to fragment shader
    normalPerVertex = vec3(model * vec4(normal, 0.0f));;
	
	// vertex position in world space
	vertexPositionInWorldSpace = vec3(model * vec4(position, 1.0f));
	
	// pass coordinates to fragment shader
	outTextureCoord = textureCoord;
	
	// transform vertex
    gl_Position = projection * view * model * vec4(position, 1.0f);
}