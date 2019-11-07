#version 330 core 

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(aPos.x, aPos.y, 0, 1);
    texCoords = aTexCoords;
}
