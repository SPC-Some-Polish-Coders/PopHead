#version 330 core 

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D image;
uniform vec4 color;

void main()
{
    fragColor = texture(image, texCoords) * color;
}
