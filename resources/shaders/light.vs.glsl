#version 330 core

layout (location = 0) in vec2 aPos;

layout (std140) uniform SharedData
{
	mat4 viewProjectionMatrix;
};

void main()
{
	gl_Position = viewProjectionMatrix * vec4(aPos, 0.0, 1.0);
}