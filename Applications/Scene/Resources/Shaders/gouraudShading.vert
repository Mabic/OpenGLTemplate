#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;

layout(std140) uniform Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shinines;	
};

layout(std140) uniform Light {
    vec4 lightPosition;
    vec4 lightColor;
};

out vec3 colorPerVertex;

void main()
{
    // calculate ambient color
    vec3 ambientColor = vec3(ambient * lightColor);

    // normalize normals
    vec3 normalizedNormals = vec3(model * vec4(normalize(normal), 0.0f));

    // calculate vertex in World Space
    vec3 vertexWorldSpacePosition = vec3(model * vec4(position, 1.0f));

    // calculate light direction
    vec3 lightDirection = normalize(vec3(lightPosition) - vertexWorldSpacePosition);

    // calculate diffuse color
    float diffuse_ = max(dot(normalizedNormals, lightDirection), 0.0f);
    vec3 diffuseColor = vec3(diffuse * lightColor) * diffuse_;

    // Specular
    vec3 viewDirection = normalize(cameraPosition - vertexWorldSpacePosition);
    vec3 reflectDirection = reflect(-lightDirection, normalizedNormals);  
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shinines);
    vec3 specularColor = vec3(lightColor * spec * specular);

    gl_Position = projection * view * model * vec4(position, 1.0f);
    colorPerVertex = ambientColor + diffuseColor + specularColor;
}