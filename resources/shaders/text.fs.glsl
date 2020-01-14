#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D image;
uniform vec4 color;

void main()
{
	float alpha = texture(image, texCoords).r * color.a;
	if(alpha < 0.1)
		discard;
	fragColor = vec4(color.rgb, alpha * color.a);
}

