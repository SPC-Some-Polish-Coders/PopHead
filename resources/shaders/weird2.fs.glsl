#version 330 core

in DATA
{
    vec4 color;
    vec2 texCoords;
    flat int textureSlotRef; 
} fs_in;

out vec4 fragColor;

uniform sampler2D textures[32];

void main()
{
    fragColor = texture(textures[fs_in.textureSlotRef], fs_in.texCoords) * vec4(fs_in.texCoords.x, fs_in.texCoords.y, 1, 1) * fs_in.color;
}
