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
    if(fs_in.color.a < 0.05)
        discard;
    fragColor = texture(textures[fs_in.textureSlotRef], fs_in.texCoords) * fs_in.color;
}
