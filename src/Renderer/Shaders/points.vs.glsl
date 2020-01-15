R"(

#version 330 core 

layout (location = 0) in vec4 aColor;
layout (location = 1) in vec2 aPos;
layout (location = 2) in float aSize;
layout (location = 3) in float aZ;

layout (std140) uniform SharedData
{
    mat4 viewProjectionMatrix;
	mat4 guiViewProjectionMatrix;
};

out vec4 color;

void main()
{
    color = aColor;
    gl_Position = viewProjectionMatrix * vec4(aPos, aZ, 1.0);
    gl_PointSize = aSize;
}

)"

