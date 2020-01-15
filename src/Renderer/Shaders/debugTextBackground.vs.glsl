R"(
#version 330 core

layout (location = 0) in vec2 aPos;

layout (std140) uniform SharedData
{
	mat4 viewProjectionMatrix;
	mat4 guiViewProjectionMatrix;
};

void main()
{
	const float z = 10 / 255;
	gl_Position = guiViewProjectionMatrix * vec4(aPos, z, 1);
}

)"