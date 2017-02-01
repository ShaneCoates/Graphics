#version 410

in vec4 Colour;
in vec2 TexCoords;
in float Time;

out vec4 fragColour;
uniform sampler2D startTexture;
uniform sampler2D endTexture;

void main()
{
	fragColour = mix(texture(startTexture, TexCoords), texture(endTexture, TexCoords), Time) * Colour;
}