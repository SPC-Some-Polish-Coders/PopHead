R"(

#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

layout (std140) uniform SharedData
{
    mat4 viewProjectionMatrix;
};

void main()
{
	gl_Position = viewProjectionMatrix * vec4(aPos, 0, 1); 
	texCoords = aTexCoords;
}

)"

