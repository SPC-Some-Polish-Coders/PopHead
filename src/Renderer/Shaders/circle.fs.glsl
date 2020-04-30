R"(

#version 330 core 

in DATA
{
    vec4 color;
    vec2 texCoords;
	vec2 texSize;
	vec2 fragPos;
	vec2 quadrantSize;
    flat int textureSlotRef; 
} fs_in;

out vec4 fragColor;

uniform sampler2D textures[32];
uniform bool debugVisualization;

void main()
{
	float dist = distance(fs_in.quadrantSize, fs_in.fragPos);
	if(dist > fs_in.quadrantSize.x || dist > fs_in.quadrantSize.y)
	{
		discard;
	}

    fragColor = texture(textures[fs_in.textureSlotRef], fs_in.texCoords) * fs_in.color;

	if(debugVisualization)
	{
		fragColor *= vec4(0.2, 0.2, 5, 1);
	}
}

// TODO: Make alpha be set in the smart way

)"

