#pragma once
#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <iostream>
#include <glm/glm.hpp>
#include <shader_s.h>
#include "Block.h"

#define chunkWidth 16
#define chunkHeight 5
#define chunkDepth 16

class ChunkManager
{
public:
	ChunkManager();
	void draw(Shader& shader, unsigned int VAO);

private:
	Block blocks[chunkWidth][chunkHeight][chunkDepth];
	unsigned int VAO;
	unsigned int VBO;

};

#endif 