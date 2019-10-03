#version 330 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D myTexture;

void main()
{
	FragColor = texture(myTexture, texCoords);
};
