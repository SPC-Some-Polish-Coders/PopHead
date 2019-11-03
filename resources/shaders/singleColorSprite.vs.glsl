#version 330 core  

layout (location = 0) in vec2 aPosition;

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;

void main()
{
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(aPosition.x, aPosition.y, 0, 1);
}
