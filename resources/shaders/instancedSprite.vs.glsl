#version 330 core 

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aModelPos;

uniform mat4 viewProjectionMatrix;

void main()
{
    gl_Position = vec4(aPos + aModelPos, 0.0, 1.0);
}
