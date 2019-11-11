#version 330 core 

layout (location = 0) in vec2 aPos;

out vec4 color;
out vec2 texCoords;
//out uint textureSlotRef;

uniform mat4 viewProjectionMatrix;

uniform vec2[100] offsets;
uniform vec2[100] sizes;
uniform vec4[100] colors;
uniform vec4[100] textureRects;
uniform uint[100] textureSlotRefs;

void main()
{
    color = colors[gl_InstanceID];
    //textureSlotRef = textureSlotRefs[gl_InstanceID];

    vec2 vertexPos;
    vec2 size = sizes[gl_InstanceID];
    vec4 tetxureRect = textureRects[gl_InstanceID];
    
    if(gl_VertexID == 0)
    {
        vertexPos = vec2(0, 0);
        texCoords = vec2(tetxureRect.x, tetxureRect.y);
    }
    else if(gl_VertexID == 1)
    {
        vertexPos = vec2(size.x, 0);
        texCoords = vec2(tetxureRect.x + tetxureRect.z, tetxureRect.y);
    }
    else if(gl_VertexID == 2)
    {
        vertexPos = vec2(size.x, size.y);
        texCoords = vec2(tetxureRect.x + tetxureRect.z, tetxureRect.y + tetxureRect.w);
    }
    else if(gl_VertexID == 3)
    {
        vertexPos = vec2(0, size.y);
        texCoords = vec2(tetxureRect.x, tetxureRect.y + tetxureRect.w);
    }

    gl_Position = viewProjectionMatrix * vec4(vertexPos + offsets[gl_InstanceID], 0.0, 1.0);
}
