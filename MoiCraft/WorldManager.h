#pragma once
#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <shader_s.h>
#include "ChunkManager.h"
#include "Camera.h"
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>


#define RENDER_DISTANCE 5

struct ChunkCoord {
	int x;
	int z;

	ChunkCoord() : x(0), z(0) {}
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
			return h1 ^ (h2 << 1); 
		}
	};
}

struct ChunkData
{
	ChunkCoord coord;
	ChunkManager* chunk;
};


class WorldManager
{
public:
	WorldManager();
	~WorldManager();

	void initialize();
	void update(const Camera& camera, float deltaTime);
	void draw(Shader& shader);

	void loadChunk(int x, int z);
	void unloadChunk(int playerChunkPosX, int playerChunkPosZ);
	
	bool isBlockAir(const glm::ivec3& worldPos);
private:
	std::unordered_map<ChunkCoord, ChunkManager*> chunks;
	unsigned int worldSeed;
	std::queue<ChunkCoord> chunkLoadQueue;
	std::vector<ChunkCoord> chunkLoadStagedCurrent;
	std::vector<ChunkCoord> chunkLoadStagedLast;
	std::unordered_set<ChunkCoord> chunkLoadQueuedSet;
	std::vector<ChunkCoord> dirtyChunks;
	int chunksPerFrame = 10;
	float chunkLoadCooldown = 0.1f; 
	float chunkLoadTimer = 0.0f;

	std::thread chunkWorker;
	std::queue<ChunkCoord> chunkLoadRequestQueue;
	std::queue<ChunkData> readyChunks;

	std::mutex chunkQueueMutex;
	std::condition_variable chunkQueueCV;
	std::atomic<bool> terminateWorker = false;

};

#endif