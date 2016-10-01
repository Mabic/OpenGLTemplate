#version 430 core

in vec3 outColor;
in vec2 outTexCords;

out vec4 color;

uniform sampler2D wallTexture;
uniform sampler2D lenaTexture;

void main(void)
{
    color = mix(texture(lenaTexture, outTexCords), texture(wallTexture, outTexCords), 0.8f) * vec4(outColor,1.0f);
}