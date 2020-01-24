R"(
#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D image;

void main()
{
	fragColor = texture(image, texCoords);
}

)"

