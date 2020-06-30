R"(

#version 330 core 

layout (location = 0) in vec4 aColor;
layout (location = 1) in vec2 aPos;

out vec4 color;

layout (std140) uniform SharedData
{   
    mat4 gameWorldVPM;
	mat4 guiVMP;
};

void main()
{
    color = aColor;
    gl_Position = gameWorldVPM * vec4(aPos, 0, 1);
}

)"
