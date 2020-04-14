R"(
#version 330 core

in vec2 uv;
out vec4 fragColor;
uniform sampler2D sampler;
uniform bool debugVisualization;

void main()
{
	const float texSideSize = 576;
	const float alpha = 0.1;

	vec2 posWithinTexel = fract(uv);
	vec2 interpolation = clamp(posWithinTexel / alpha, 0.0, 0.5) + clamp((posWithinTexel - 1.0) / alpha + 0.5, 0.0, 0.5);
	vec2 finalTexCoords = (floor(uv) + interpolation) / texSideSize; 
	fragColor = texture(sampler, finalTexCoords);

	if(debugVisualization)
	{
		fragColor *= vec4(5, 0.2, 0.2, 1);
	}
}

// TODO: Make alpha be set in the smart way

)"
