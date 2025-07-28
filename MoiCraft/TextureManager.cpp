#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureManager::TextureManager()
{
    textures[BlockType::Grass] = "C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/grass_top.jpg";
    textures[BlockType::Dirt] = "C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/perfect_dirt.jpg";
    textures[BlockType::Stone] = "C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/stone.jpg";
}

void TextureManager::AddTextures()
{
   
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/grass_top.jpg");
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/perfect_dirt.jpg");
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/stone.jpg");
    InitializeTextures("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/grass_side.jpg");
    
}

void TextureManager::InitializeTextures(const char* texturePath)
{
    std::cout << "Loading texture from path: " << texturePath << std::endl;
    unsigned int textureID;
    // texture 1
    // ---------
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
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
    
    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

    ourShader.setInt("texture_GrassTop", 0);
    ourShader.setInt("texture_GrassSide", 1);
    */
}

unsigned int TextureManager::GetTexturesSize()
{
    return textures.size();
    //return 5;
}
