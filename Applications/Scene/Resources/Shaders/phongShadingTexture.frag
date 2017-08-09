#version 430 core

uniform vec3 cameraPosition;
uniform sampler2D textureSampler;

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

in vec3 normalPerVertex;
in vec3 vertexPositionInWorldSpace;
in vec2 outTextureCoord;
out vec4 outColor;

void main(void)
{
    // calculate ambient color
    vec3 ambientColor = vec3(ambient * lightColor);

    // normalize normals
    vec3 normalizedNormals = normalize(normalPerVertex);

    // calculate light direction
	vec3 lightDirection;
	if (lightPosition.w < 0.05f) {
		lightDirection = vec3(normalize(-lightPosition));
	} else {
		lightDirection = normalize(vec3(lightPosition) - vertexPositionInWorldSpace);
	}

    // calculate diffuse color
    float diffuse_ = max(dot(normalizedNormals, lightDirection), 0.0f);
    vec3 diffuseColor = vec3(texture2D(textureSampler, outTextureCoord) * lightColor) * diffuse_;

    // Specular
    vec3 viewDirection = normalize(cameraPosition - vertexPositionInWorldSpace);
    vec3 reflectDirection = reflect(-lightDirection, normalizedNormals);  
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shinines);
    vec3 specularColor = vec3(lightColor * spec * specular);

    outColor = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}