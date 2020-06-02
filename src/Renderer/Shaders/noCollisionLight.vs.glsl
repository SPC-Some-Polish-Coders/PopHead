R"(

#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 lightPos;

layout (std140) uniform SharedData
{
	mat4 gameWorldVPM;
	mat4 guiVPM;
};

out DATA
{
	vec2 fragPos;
	flat vec2 lightPos;
} vs_out;

void main()
{
	gl_Position = vec4(aPos, 0, 1); 
	vs_out.fragPos = aPos;
	vs_out.lightPos = vec2(gameWorldVPM * vec4(lightPos, 0, 1));
}


)"
