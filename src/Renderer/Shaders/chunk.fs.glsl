R"(
#version 330 core

in vec2 texCoords;
out vec4 fragColor;
uniform sampler2D sampler;

void main()
{
	const float texSideSize = 576;
	const float alpha = 0.1;

	vec2 posWithinTexel = fract(texCoords);
	vec2 interpolation = clamp(posWithinTexel / alpha, 0.0, 0.5) + clamp((posWithinTexel - 1.0) / alpha + 0.5, 0.0, 0.5);
	vec2 finalTexCoords = (floor(texCoords) + interpolation) / texSideSize; 
	fragColor = texture(sampler, finalTexCoords);
}

// TODO: Make alpha be set in the smart way

)"
