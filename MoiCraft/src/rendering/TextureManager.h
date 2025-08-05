#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <string>
#include "Block.h"
#include <shader_s.h>

class TextureManager
{
public:
	void AddTextures();
	void ActivateTexture(Shader& ourShader);

private:
	void InitializeTextures(const char* texturePath);

private:
	std::unordered_map<BlockType, const char*> textures;
	std::vector<unsigned int> textureIDs;
};

#endif 
