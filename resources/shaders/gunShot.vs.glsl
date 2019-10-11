#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	gl_Position = viewProjectionMatrix * vec4(aPos.x, aPos.y, 0.0, 1.0);
};
