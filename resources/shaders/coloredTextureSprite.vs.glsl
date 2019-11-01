#version 330 core 

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTextureCoords;

out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
    texCoords = aTextureCoords;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(aPosition.x, aPosition.y, 0, 1);
}
