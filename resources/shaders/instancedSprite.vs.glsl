#version 330 core 

layout (location = 0) in vec2 aOffset;

out vec4 color;
out vec2 texCoords;
flat out int textureSlotRef;

uniform mat4 viewProjectionMatrix;

uniform vec2[100] sizes;
uniform float[100] rotations;
uniform vec4[100] colors;
uniform vec4[100] textureRects;
uniform int[100] textureSlotRefs;

mat2 getRotationMatrix(float angle);

void main()
{
    color = colors[gl_InstanceID];
    textureSlotRef = textureSlotRefs[gl_InstanceID];

    vec2 size = sizes[gl_InstanceID];
    float rotation = rotations[gl_InstanceID];
    vec4 tetxureRect = textureRects[gl_InstanceID];

    vec2 modelVertexPos;
    
    // TODO: Mauye change that to switch
    if(gl_VertexID == 0) {
        texCoords = vec2(tetxureRect.x, tetxureRect.y);
        modelVertexPos = vec2(0, 0);
    }
    else if(gl_VertexID == 1) {
        texCoords = vec2(tetxureRect.x + tetxureRect.z, tetxureRect.y);
        modelVertexPos = vec2(size.x, 0);
    }
    else if(gl_VertexID == 2) {
        texCoords = vec2(tetxureRect.x + tetxureRect.z, tetxureRect.y + tetxureRect.w);
        modelVertexPos = vec2(size.x, size.y);
    }
    else if(gl_VertexID == 3) {
        texCoords = vec2(tetxureRect.x, tetxureRect.y + tetxureRect.w);
        modelVertexPos = vec2(0, size.y);
    }

    if(rotation == 0)
        gl_Position = viewProjectionMatrix * vec4(modelVertexPos + aOffset, 0, 1);
    else
        gl_Position = viewProjectionMatrix * vec4(modelVertexPos * getRotationMatrix(rotation) + aOffset, 0, 1);
}

mat2 getRotationMatrix(float angle)
{
    return mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );
}
