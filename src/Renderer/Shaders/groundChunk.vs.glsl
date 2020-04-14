R"(
#version 330 core

layout (location = 0) in vec2 aVertexOffset;

uniform vec2 chunkPos;
uniform float z;

uniform vec2 uvTopLeft; 
uniform vec2 uvTopRight; 
uniform vec2 uvBottomLeft; 
uniform vec2 uvBottomRight;

out vec2 uv;

layout (std140) uniform SharedData
{
    mat4 gameWorldVPM;
	mat4 guiVPM;
};

void main()
{
	switch(gl_VertexID)
	{
		case 0: uv = uvTopLeft; break;
		case 1: uv = uvTopRight; break;
		case 2: uv = uvBottomLeft; break;
		case 3: uv = uvBottomRight; break;
	}
	uv *= 576;

	vec2 chunkRelPos;
	chunkRelPos.x = (gl_InstanceID * 16) % (16 * 12);
	chunkRelPos.y = floor(gl_InstanceID / 12) * 16;
	gl_Position = gameWorldVPM * vec4(chunkPos + chunkRelPos + aVertexOffset, z, 1); 
}

)"
