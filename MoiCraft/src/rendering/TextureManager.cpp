#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void TextureManager::AddTextures()
{
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/assets/textures/grass_top.jpg");
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/assets/textures/perfect_dirt.jpg");
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/assets/textures/stone.jpg");
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/assets/textures/grass_side.jpg");
}

void TextureManager::InitializeTextures(const char* texturePath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        textureIDs.push_back(textureID);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void TextureManager::ActivateTexture(Shader& ourShader)
{
    for (unsigned int i = 0; i < textureIDs.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
        if (i == 0)
            ourShader.setInt("texture_GrassTop", 0);
        else if (i == 1)
            ourShader.setInt("texture_Dirt", 1);
        else if (i == 2)
            ourShader.setInt("texture_Stone", 2);
        else if (i == 3)
            ourShader.setInt("texture_GrassSide", 3);
    }
}

