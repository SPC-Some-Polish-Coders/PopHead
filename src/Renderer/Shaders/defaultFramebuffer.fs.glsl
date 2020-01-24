R"(
#version 330 core 

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D gameObjectsTexture;
uniform sampler2D lightingTexture;

void main()
{
    fragColor = texture(gameObjectsTexture, texCoords) * texture(lightingTexture, texCoords);
}

)"

