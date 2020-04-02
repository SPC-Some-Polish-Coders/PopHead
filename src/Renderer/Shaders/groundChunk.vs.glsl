R"(
#version 330 core

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

// TODO: Add pixel art texture filtering

void main()
{
	vec2 vertexOffset;
	switch(gl_VertexID)
	{
		case 0: {
			vertexOffset = vec2(0, 0);
			uv = uvTopLeft;
		} break;

		case 1: {
			vertexOffset = vec2(16, 0);
			uv = uvTopRight;
		} break;

		case 2: {
			vertexOffset = vec2(0, 16);
			uv = uvBottomLeft;
		} break;

		case 3: {
			vertexOffset = vec2(16, 16);
			uv = uvBottomRight;
		} break;
	}

	vec2 chunkRelPos;
	chunkRelPos.x = (gl_InstanceID * 16) % (16 * 12);
	chunkRelPos.y = floor(gl_InstanceID / 12) * 16;
	gl_Position = gameWorldVPM * vec4(chunkPos + chunkRelPos + vertexOffset, z, 1); 
}

)"
