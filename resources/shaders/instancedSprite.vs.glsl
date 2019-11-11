#version 330 core 

layout (location = 0) in vec2 aPos;

out vec4 color;

uniform mat4 viewProjectionMatrix;
uniform vec2[100] offsets;
uniform vec2[100] sizes;
uniform vec4[100] colors;

void main()
{
    color = colors[gl_InstanceID];

    vec2 vertexPos;
    vec2 size = sizes[gl_InstanceID];
    if(gl_VertexID == 0)
        vertexPos = vec2(0, 0);
    else if(gl_VertexID == 1)
        vertexPos = vec2(size.x, 0);
    else if(gl_VertexID == 2)
        vertexPos = vec2(size.x, size.y);
    else if(gl_VertexID == 3)
        vertexPos = vec2(0, size.y);

    gl_Position = viewProjectionMatrix * vec4(vertexPos + offsets[gl_InstanceID], 0.0, 1.0);
}
