#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 mTextureCoords;

out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
	mat4 newModelMatrx = modelMatrix;
	newModelMatrx[3][0] /= 640;
	newModelMatrx[3][1] /= 480;
	gl_Position = viewProjectionMatrix * newModelMatrx * vec4(aPos.x, aPos.y, 0.0, 1.0);
	texCoords = mTextureCoords;
};
