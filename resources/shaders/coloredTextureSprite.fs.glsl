#version 330 core 

in vec2 texCoords;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D image;

void main()
{
    FragColor = color * texture(image, texCoords);
}
