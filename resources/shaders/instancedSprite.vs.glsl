#version 330 core 

layout (location = 0) in vec2 aOffset;
layout (location = 1) in vec2 aSize;
layout (location = 2) in float aRotation;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec4 aTextureRect;

out VS_OUT
{
    vec4 color;
    vec2 texCoords;
    flat int textureSlotRef;
} vs_out;

uniform mat4 viewProjectionMatrix;

uniform int[2500] textureSlotRefs;

mat2 getRotationMatrix(float angle);

void main()
{
    vs_out.color = aColor;
    vs_out.textureSlotRef = textureSlotRefs[gl_InstanceID];

    vec2 modelVertexPos;
    
    switch(gl_VertexID)
    {
        case 0:
            modelVertexPos = vec2(0, 0);
            vs_out.texCoords = vec2(aTextureRect.x, aTextureRect.y + aTextureRect.w);
            break;
        case 1:
            modelVertexPos = vec2(aSize.x, 0);
            vs_out.texCoords = vec2(aTextureRect.x + aTextureRect.z, aTextureRect.y + aTextureRect.w);
            break;
        case 2:
            modelVertexPos = vec2(aSize.x, aSize.y);
            vs_out.texCoords = vec2(aTextureRect.x + aTextureRect.z, aTextureRect.y);
            break;
        case 3:
            vs_out.texCoords = vec2(aTextureRect.x, aTextureRect.y);
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
