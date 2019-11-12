#version 330 core 

in vec4 color;
in vec2 texCoords;
flat in int textureSlotRef; 

out vec4 fragColor;

uniform sampler2D textures[16];

void main()
{
    fragColor = texture(textures[textureSlotRef], texCoords) * color;
}
