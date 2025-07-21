#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
flat in int faceID;


uniform sampler2D texture1;
uniform sampler2D texture2;

uniform sampler2D texture_GrassTop;
uniform sampler2D texture_GrassSide;
uniform sampler2D texture_Dirt;
void main()
{

    if (faceID == 0)
        FragColor = texture(texture_GrassTop, TexCoord);
    else if (faceID == 1)
        FragColor = texture(texture_Dirt, TexCoord);
    else
        FragColor = texture(texture_GrassSide, TexCoord);

    //FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.0);
}