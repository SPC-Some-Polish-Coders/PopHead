#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 mTextureCoords;

out vec2 texCoords;

uniform mat4 modelMatrix;

void main()
{
	gl_Position = modelMatrix * vec4(aPos, 0.0, 1.0);
	texCoords = mTextureCoords;
};
