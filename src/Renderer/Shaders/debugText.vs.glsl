R"(
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 debugTextViewProjectionMatrix;

void main()
{
	texCoords = aTexCoords;
	gl_Position = debugTextViewProjectionMatrix * vec4(aPos, 0, 1);	
}

)"
