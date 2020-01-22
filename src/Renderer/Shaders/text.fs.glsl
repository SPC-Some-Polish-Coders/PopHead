R"(

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
    fragColor = vec4(fs_in.color.rgb, texture(textures[fs_in.textureSlotRef], fs_in.texCoords).r * fs_in.color.a);
}

// TODO: Make alpha be set in the smart way

)"

