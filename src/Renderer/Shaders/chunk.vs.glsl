R"(
#version 330 core

layout (location = 0) in vec4 aTextureRect;
layout (location = 1) in vec2 aWorldPos;
layout (location = 2) in vec2 aSize;
layout (location = 3) in float aRotation;

layout (std140) uniform SharedData
{
    mat4 gameWorldVPM;
	mat4 guiVPM;
};

uniform float z;

out vec2 texCoords;

void main()
{	
	vec2 vertexOffset;
	switch(gl_VertexID)
	{
		case 0: {
            vertexOffset = vec2(0, 0);
            texCoords = vec2(aTextureRect.x, aTextureRect.y + aTextureRect.w);
        } break;
        
		case 1: {
			vertexOffset = vec2(aSize.x, 0);
            texCoords = vec2(aTextureRect.x + aTextureRect.z, aTextureRect.y + aTextureRect.w);
        } break;

        case 2: {
            vertexOffset = vec2(0, aSize.y);
            texCoords = vec2(aTextureRect.x, aTextureRect.y);
        } break;

        case 3: {
            vertexOffset = vec2(aSize.x, aSize.y);
            texCoords = vec2(aTextureRect.x + aTextureRect.z, aTextureRect.y);
        } break;
	}
	
	texCoords *= 576;

	if(aRotation == 0)
	{
		gl_Position = gameWorldVPM * vec4(aWorldPos + vertexOffset, z, 1);
	}
	else
	{
		mat2 rotationMatrix = mat2(
			cos(aRotation), -sin(aRotation),
			sin(aRotation), cos(aRotation)
		);
		vec2 rotatedVertexPos = (vertexOffset - vec2(8, 8)) * rotationMatrix + aWorldPos + vec2(8, 8);
		gl_Position = gameWorldVPM * vec4(rotatedVertexPos, z, 1);
	}
}

)"
