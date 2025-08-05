#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
flat in int blockType;
flat in int faceID;


uniform sampler2D texture1;
uniform sampler2D texture2;

uniform sampler2D texture_GrassTop;
uniform sampler2D texture_GrassSide;
uniform sampler2D texture_Dirt;
uniform sampler2D texture_Stone;
void main()
{

    FragColor = vec4(1.0, 0.0, 1.0, 1.0);

    if (blockType == 1) 
    { 
        if (faceID == 0)
            FragColor = texture(texture_GrassTop, TexCoord);
        else if (faceID == 1)
            FragColor = texture(texture_Dirt, TexCoord);
        else
            FragColor = texture(texture_GrassSide, TexCoord);
    }
    else if (blockType == 2) 
    { 
        FragColor = texture(texture_Dirt, TexCoord);
    }
    else if (blockType == 3) 
    { 
        FragColor = texture(texture_Stone, TexCoord);
    }
}