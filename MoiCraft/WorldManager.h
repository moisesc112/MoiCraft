#pragma once
#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <iostream>
#include <unordered_map>
#include <shader_s.h>
#include "ChunkManager.h"
#include "Camera.h"
#include <random>
#include <ctime>
#include <unordered_set>

#define RENDER_DISTANCE 5

struct ChunkCoord {
	int x;
	int z;

	ChunkCoord(int x_, int z_) : x(x_), z(z_) {}

	bool operator==(const ChunkCoord& other) const {
		return x == other.x && z == other.z;
	}
};

namespace std {
	template <>
	struct hash<ChunkCoord> {
		std::size_t operator()(const ChunkCoord& coord) const {
			std::size_t h1 = std::hash<int>()(coord.x);
			std::size_t h2 = std::hash<int>()(coord.z);
			return h1 ^ (h2 << 1); // Combine hashes using bitwise ops
		}
	};
}

class WorldManager
{
public:
	WorldManager();
	~WorldManager();
	void draw(Shader& shader);
	void update(const Camera& camera);
	void loadChunk(int x, int z, bool initialize);
	void unloadChunk(int playerChunkPosX, int playerChunkPosZ);
	Block* getBlock(float x, float y, float z);
	bool isBlockAir(const glm::ivec3& worldPos);
private:
	std::unordered_map<ChunkCoord, ChunkManager*> chunks;
	unsigned int worldSeed;
};

#endif