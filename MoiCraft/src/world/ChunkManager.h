#pragma once
#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Block.h"
#include "shader_s.h"
#include "PerlinNoise/PerlinNoise.hpp"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 20
#define CHUNK_DEPTH 16

class WorldManager;

class ChunkManager
{
public:
	ChunkManager(glm::ivec3 position, unsigned int seed);
	~ChunkManager();

	void setWorld(WorldManager* worldManager);

	void initializeMesh();
	void generateMesh();
	void draw(Shader& shader);

	unsigned int getVAO() const;
	unsigned int getVBO() const;

	void deleteVAO();
	void deleteVBO();

	bool isFaceVisible(const glm::ivec3& position, const glm::ivec3& direction);
	bool isBlockAir(const glm::ivec3& localPos);

	void markDirty();

	Block& getBlock(int x, int y, int z);
private:
	void addFace(const std::vector<float>& faceData, const glm::ivec3& position);

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