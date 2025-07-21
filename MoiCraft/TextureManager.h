#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <string>
#include "Block.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>


class TextureManager
{
	public:
		TextureManager();
		void AddTextures();
		void InitializeTextures(const char* texturePath);
		void ActivateTexture(Shader& ourShader);
		unsigned int GetTexturesSize();
	private:
		std::unordered_map<BlockType, const char*> textures;
		std::vector<unsigned int> textureIDs;
		//Shader ourShader;

};

#endif 
