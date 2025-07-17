#pragma once
#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <iostream>
#include <glm/glm.hpp>
#include <shader_s.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Block.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 5
#define CHUNK_DEPTH 16

class ChunkManager
{
public:
	ChunkManager();
	ChunkManager(glm::vec3 position);
	void generateMesh();
	void draw(Shader& shader);
	unsigned int getVAO();
	unsigned int getVBO();
	void deleteVAO();
	void deleteVBO();

private:
	Block block;
	Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
	unsigned int VAO;
	unsigned int VBO;
	glm::vec3 chunkPosition;
	std::vector<float> vertices;

	void addFace(std::vector<float> faceData, glm::vec3 position);
	bool isFaceVisible(int x, int y, int z, glm::vec3 direction);
};

#endif 