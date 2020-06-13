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
	// NOTE: We do this bizzare switch because the normal solution doesn't work on AMD GPUs (:
	float r;
	switch(fs_in.textureSlotRef)
	{
		case 0: r = texture(textures[0], fs_in.texCoords).r; break;
		case 1: r = texture(textures[1], fs_in.texCoords).r; break;
		case 2: r = texture(textures[2], fs_in.texCoords).r; break;
		case 3: r = texture(textures[3], fs_in.texCoords).r; break;
		case 4: r = texture(textures[4], fs_in.texCoords).r; break;
		case 5: r = texture(textures[5], fs_in.texCoords).r; break;
		case 6: r = texture(textures[6], fs_in.texCoords).r; break;
		case 7: r = texture(textures[7], fs_in.texCoords).r; break;
		case 8: r = texture(textures[8], fs_in.texCoords).r; break;
		case 9: r = texture(textures[9], fs_in.texCoords).r; break;
		case 10: r = texture(textures[10], fs_in.texCoords).r; break;
		case 11: r = texture(textures[11], fs_in.texCoords).r; break;
		case 12: r = texture(textures[12], fs_in.texCoords).r; break;
		case 13: r = texture(textures[13], fs_in.texCoords).r; break;
		case 14: r = texture(textures[14], fs_in.texCoords).r; break;
		case 15: r = texture(textures[15], fs_in.texCoords).r; break;
		case 16: r = texture(textures[16], fs_in.texCoords).r; break;
		case 17: r = texture(textures[17], fs_in.texCoords).r; break;
		case 18: r = texture(textures[18], fs_in.texCoords).r; break;
		case 19: r = texture(textures[19], fs_in.texCoords).r; break;
		case 20: r = texture(textures[20], fs_in.texCoords).r; break;
		case 21: r = texture(textures[21], fs_in.texCoords).r; break;
		case 22: r = texture(textures[22], fs_in.texCoords).r; break;
		case 23: r = texture(textures[23], fs_in.texCoords).r; break;
		case 24: r = texture(textures[24], fs_in.texCoords).r; break;
		case 25: r = texture(textures[25], fs_in.texCoords).r; break;
		case 26: r = texture(textures[26], fs_in.texCoords).r; break;
		case 27: r = texture(textures[27], fs_in.texCoords).r; break;
		case 28: r = texture(textures[28], fs_in.texCoords).r; break;
		case 29: r = texture(textures[29], fs_in.texCoords).r; break;
		case 30: r = texture(textures[30], fs_in.texCoords).r; break;
		case 31: r = texture(textures[31], fs_in.texCoords).r; break;
	}

    fragColor = vec4(fs_in.color.rgb, r * fs_in.color.a);
}

)"

