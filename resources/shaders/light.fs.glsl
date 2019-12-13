#version 330 core

out vec4 fragColor;

uniform vec4 color;

void main()
{
	fragColor = color * vec4(1, 1, 1, 0.5); 
}
