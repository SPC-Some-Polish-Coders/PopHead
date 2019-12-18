#version 330 core 

layout (location = 0) in vec4 aColor;
layout (location = 1) in vec4 aTextureRect;
layout (location = 2) in vec2 aPosition;
layout (location = 3) in vec2 aSize;
layout (location = 4) in float aTextureSlotRef;
layout (location = 5) in float aRotation;

out DATA
{
    vec4 color;
    vec2 texCoords;
	vec2 texSize;
    flat int textureSlotRef;
} vs_out;

layout (std140) uniform SharedData
{
    mat4 viewProjectionMatrix;
};

uniform float z;
uniform sampler2D textures[32];

mat2 getRotationMatrix(float angle);

void main()
{
    vs_out.color = aColor;
    vs_out.textureSlotRef = int(aTextureSlotRef);

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
            modelVertexPos = vec2(0, aSize.y);
            vs_out.texCoords = vec2(aTextureRect.x, aTextureRect.y);
            break;
    }

	vs_out.texSize = vec2(textureSize(textures[int(aTextureSlotRef)], 0));
	vs_out.texCoords *= vs_out.texSize;
    
    if(aRotation == 0)
        gl_Position = viewProjectionMatrix * vec4(modelVertexPos + aPosition, z, 1);
    else
        gl_Position = viewProjectionMatrix * vec4(modelVertexPos * getRotationMatrix(aRotation) + aPosition, z, 1);
}

mat2 getRotationMatrix(float angle)
{
    return mat2(
        cos(angle), -sin(angle),
        sin(angle),  cos(angle)
    );
}
