#pragma once
#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <iostream>
#include <glm/glm.hpp>
#include <shader_s.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Block.h"
#include "PerlinNoise.hpp"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 20
#define CHUNK_DEPTH 16

class WorldManager;

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();
	ChunkManager(glm::ivec3 position, unsigned int seed);
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
	void markDirty();
	inline Block& getBlock(int x, int y, int z);
private:
	Block block;
	std::vector<Block> blocks;
	unsigned int VAO;
	unsigned int VBO;
	glm::ivec3 chunkPosition;
	std::vector<float> vertices;
	std::vector<std::vector<float>> faceData;
	WorldManager* world;
	siv::PerlinNoise noiseGenerator;
	bool meshInitialized = false;
	bool dirty = false;
};

#endif 