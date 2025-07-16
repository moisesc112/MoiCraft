#pragma once
#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <iostream>
#include <glm/glm.hpp>
#include <shader_s.h>
#include "Block.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 20
#define CHUNK_DEPTH 16

class ChunkManager
{
public:
	ChunkManager();
	ChunkManager(glm::vec3 position);
	void draw(Shader& shader);
	unsigned int getVAO();
	unsigned int getVBO();
	void deleteVAO();
	void deleteVBO();

private:
	Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
	unsigned int VAO;
	unsigned int VBO;
	glm::vec3 chunkPosition;
};

#endif 