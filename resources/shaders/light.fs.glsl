#version 330 core

in DATA
{
	vec2 fragPos;
	flat vec2 lightPos;
} fs_in;

out vec4 fragColor;

uniform vec4 color;
uniform float a; // attenuation addition 
uniform float b; // attenuation factor
uniform float c; // attenuation square factor

void main()
{
	float dist = length(fs_in.fragPos - fs_in.lightPos);
	float lightIntensity = 1.0 / (a + b * dist + c * c * dist); 
	fragColor = color * lightIntensity;
}
