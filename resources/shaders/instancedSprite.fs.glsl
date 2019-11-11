#version 330 core 

in vec4 color;
in vec2 texCoords;
//in uint textureSlotRef; 

out vec4 fragColor;

uniform sampler2D texture0;
// uniform sampler2D texture1;
// uniform sampler2D texture2;
// uniform sampler2D texture3;
// uniform sampler2D texture4;
// uniform sampler2D texture5;
// uniform sampler2D texture6;
// uniform sampler2D texture7;
// uniform sampler2D texture8;
// uniform sampler2D texture9;
// uniform sampler2D texture10;
// uniform sampler2D texture11;
// uniform sampler2D texture12;
// uniform sampler2D texture13;
// uniform sampler2D texture14;
// uniform sampler2D texture15;

void main()
{
    fragColor = texture(texture0, texCoords) * color;

    // switch(textureSlotRef)
    // {
    //     case 0: fragColor = texture(texture0, texCoords) * color; break;
    //     case 1: fragColor = texture(texture1, texCoords) * color; break;
    //     case 2: fragColor = texture(texture2, texCoords) * color; break;
    //     case 3: fragColor = texture(texture3, texCoords) * color; break;
    //     case 4: fragColor = texture(texture4, texCoords) * color; break;
    //     case 5: fragColor = texture(texture5, texCoords) * color; break;
    //     case 6: fragColor = texture(texture6, texCoords) * color; break;
    //     case 7: fragColor = texture(texture7, texCoords) * color; break;
    //     case 8: fragColor = texture(texture8, texCoords) * color; break;
    //     case 9: fragColor = texture(texture9, texCoords) * color; break;
    //     case 10: fragColor = texture(texture10, texCoords) * color; break;
    //     case 11: fragColor = texture(texture11, texCoords) * color; break;
    //     case 12: fragColor = texture(texture12, texCoords) * color; break;
    //     case 13: fragColor = texture(texture13, texCoords) * color; break;
    //     case 14: fragColor = texture(texture14, texCoords) * color; break;
    //     case 15: fragColor = texture(texture15, texCoords) * color; break;
    // }
}
