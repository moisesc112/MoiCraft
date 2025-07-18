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
#define CHUNK_HEIGHT 2
#define CHUNK_DEPTH 16

class WorldManager;

class ChunkManager
{
public:
	ChunkManager();
	ChunkManager(glm::ivec3 position);
	void initializeMesh();
	void generateMesh();
	void draw(Shader& shader);
	unsigned int getVAO();
	unsigned int getVBO();
	void deleteVAO();
	void deleteVBO();
	bool isBlockAir(const glm::ivec3& localPos);
	void setWorld(WorldManager* worldManager);
	void addFace(const std::vector<float>& faceData, const glm::ivec3& position);
	bool isFaceVisible(const glm::ivec3& position, const glm::ivec3& direction);
private:
	Block block;
	Block blocks[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
	unsigned int VAO;
	unsigned int VBO;
	glm::ivec3 chunkPosition;
	std::vector<float> vertices;
	std::vector<std::vector<float>> faceData;
	WorldManager* world;
};

#endif 