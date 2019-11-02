#version 330 core 

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D screenImage;

void main()
{
    fragColor = texture(screenImage, texCoords);
}
