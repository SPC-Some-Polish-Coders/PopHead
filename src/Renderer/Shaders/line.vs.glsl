R"(

#version 330 core 

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 color;

layout (std140) uniform SharedData
{   
    uniform mat4 viewProjectionMatrix;
};

void main()
{
    color = aColor;
    gl_Position = viewProjectionMatrix * vec4(aPos, 0.0, 1.0);
}

)"
