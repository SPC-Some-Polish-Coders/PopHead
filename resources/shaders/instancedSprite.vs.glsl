#version 330 core 

layout (location = 0) in vec2 aOffset;
layout (location = 1) in vec2 aSize;
layout (location = 2) in float aRotation;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec4 aTextureRect;

out vec4 color;
out vec2 texCoords;
flat out int textureSlotRef;

uniform mat4 viewProjectionMatrix;

uniform int[100] textureSlotRefs;

mat2 getRotationMatrix(float angle);

void main()
{
    color = aColor;
    textureSlotRef = textureSlotRefs[gl_InstanceID];

    vec2 modelVertexPos;
    
    switch(gl_VertexID)
    {
        case 0:
            texCoords = vec2(aTextureRect.x, aTextureRect.y);
            modelVertexPos = vec2(0, 0);
            break;
        case 1:
            texCoords = vec2(aTextureRect.x + aTextureRect.z, aTextureRect.y);
            modelVertexPos = vec2(aSize.x, 0);
            break;
        case 2:
            texCoords = vec2(aTextureRect.x + aTextureRect.z, aTextureRect.y + aTextureRect.w);
            modelVertexPos = vec2(aSize.x, aSize.y);
            break;
        case 3:
            texCoords = vec2(aTextureRect.x, aTextureRect.y + aTextureRect.w);
            modelVertexPos = vec2(0, aSize.y);
            break;
    }
    
    if(aRotation == 0)
        gl_Position = viewProjectionMatrix * vec4(modelVertexPos + aOffset, 0, 1);
    else
        gl_Position = viewProjectionMatrix * vec4(modelVertexPos * getRotationMatrix(aRotation) + aOffset, 0, 1);
}

mat2 getRotationMatrix(float angle)
{
    return mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );
}
