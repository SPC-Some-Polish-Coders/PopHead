R"(

#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

layout (std140) uniform SharedData
{
    mat4 gameWorldVPM;
	mat4 guiVPM;
};

uniform bool isGameWorldProjection;

void main()
{
	mat4 VPM = isGameWorldProjection ? gameWorldVPM : guiVPM;
	gl_Position = VPM * vec4(aPos, 0, 1); 
	texCoords = aTexCoords;
}

)"

