#version 330 core

layout (location = 0) in vec2 aPos;

out DATA
{
	vec2 fragPos;
	flat vec2 lightPos;
} vs_out;

layout (std140) uniform SharedData
{
	mat4 viewProjectionMatrix;
};
uniform vec2 lightPos;

void main()
{
	vec4 vertexPos = viewProjectionMatrix * vec4(aPos, 0.0, 1.0);
	vs_out.fragPos = vertexPos.xy;
	vs_out.lightPos = vec2(viewProjectionMatrix * vec4(lightPos, 0, 1));
	gl_Position = vertexPos;
}