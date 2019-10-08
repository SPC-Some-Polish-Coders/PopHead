#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 mTextureCoords;

out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	mat4 perfectPixelModelMatrix = modelMatrix;
	perfectPixelModelMatrix[3][0] = float(int(modelMatrix[3][0]));
	perfectPixelModelMatrix[3][1] = float(int(modelMatrix[3][1]));
	gl_Position = viewProjectionMatrix * perfectPixelModelMatrix * vec4(aPos.x, aPos.y, 0.0, 1.0);
	texCoords = mTextureCoords;
};
