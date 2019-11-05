#version 330 core 

in vec2 texCoords;
in vec4 multColor;

out vec4 fragColor;

uniform sampler2D image;

void main()
{
    fragColor = texture(image, texCoords) * multColor;
}
