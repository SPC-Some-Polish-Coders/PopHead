R"(

#version 330 core 

in DATA
{
    vec4 color;
    vec2 texCoords;
	vec2 texSize;
    flat int textureSlotRef; 
} fs_in;

out vec4 fragColor;

uniform sampler2D textures[32];
uniform bool debugVisualization;

void main()
{
	// TODO: Make alpha be set in the smart way
	const float alpha = 0.1;

	vec2 locationWithinTexel = fract(fs_in.texCoords);
	vec2 interpolationAmount = clamp(locationWithinTexel / alpha, 0.0, 0.5) + clamp((locationWithinTexel - 1.0) / alpha + 0.5, 0.0, 0.5);
	vec2 finalTexCoords = (floor(fs_in.texCoords) + interpolationAmount) / fs_in.texSize; 
	
	// NOTE: We do this bizzare switch because the normal solution doesn't work on AMD GPUs (:
	vec4 texColor;
	switch(fs_in.textureSlotRef)
	{
		case 0: texColor = texture(textures[0], finalTexCoords); break;
		case 1: texColor = texture(textures[1], finalTexCoords); break;
		case 2: texColor = texture(textures[2], finalTexCoords); break;
		case 3: texColor = texture(textures[3], finalTexCoords); break;
		case 4: texColor = texture(textures[4], finalTexCoords); break;
		case 5: texColor = texture(textures[5], finalTexCoords); break;
		case 6: texColor = texture(textures[6], finalTexCoords); break;
		case 7: texColor = texture(textures[7], finalTexCoords); break;
		case 8: texColor = texture(textures[8], finalTexCoords); break;
		case 9: texColor = texture(textures[9], finalTexCoords); break;
		case 10: texColor = texture(textures[10], finalTexCoords); break;
		case 11: texColor = texture(textures[11], finalTexCoords); break;
		case 12: texColor = texture(textures[12], finalTexCoords); break;
		case 13: texColor = texture(textures[13], finalTexCoords); break;
		case 14: texColor = texture(textures[14], finalTexCoords); break;
		case 15: texColor = texture(textures[15], finalTexCoords); break;
		case 16: texColor = texture(textures[16], finalTexCoords); break;
		case 17: texColor = texture(textures[17], finalTexCoords); break;
		case 18: texColor = texture(textures[18], finalTexCoords); break;
		case 19: texColor = texture(textures[19], finalTexCoords); break;
		case 20: texColor = texture(textures[20], finalTexCoords); break;
		case 21: texColor = texture(textures[21], finalTexCoords); break;
		case 22: texColor = texture(textures[22], finalTexCoords); break;
		case 23: texColor = texture(textures[23], finalTexCoords); break;
		case 24: texColor = texture(textures[24], finalTexCoords); break;
		case 25: texColor = texture(textures[25], finalTexCoords); break;
		case 26: texColor = texture(textures[26], finalTexCoords); break;
		case 27: texColor = texture(textures[27], finalTexCoords); break;
		case 28: texColor = texture(textures[28], finalTexCoords); break;
		case 29: texColor = texture(textures[29], finalTexCoords); break;
		case 30: texColor = texture(textures[30], finalTexCoords); break;
		case 31: texColor = texture(textures[31], finalTexCoords); break;
	}

	fragColor = texColor * fs_in.color;

	if(debugVisualization)
	{
		fragColor *= vec4(0.2, 0.2, 5, 1);
	}
}

)"

