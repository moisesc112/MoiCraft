#include "WorldManager.h"

WorldManager::WorldManager()
{


	//loadChunk(0, 0, false);
	//loadChunk(1, 0, false);
	//loadChunk(1, 1, false);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			loadChunk(i, j, false);
		}
	}

	for(auto it = chunks.begin(); it != chunks.end(); ++it) {
		ChunkManager* chunk = it->second;
		chunk->setWorld(this);
	}

	for (auto it = chunks.begin(); it != chunks.end(); ++it) {
		ChunkManager* chunk = it->second;
		chunk->initializeMesh();
	}
}


void WorldManager::loadChunk(float x, float z, bool initialize)
{
	ChunkCoord chunkCoord(x, z);
	if (chunks.find(chunkCoord) == chunks.end())
	{
		chunks[chunkCoord] = new ChunkManager(glm::ivec3(x * CHUNK_WIDTH, 0.0f, z * CHUNK_DEPTH));
		if (initialize)
		{
			chunks[chunkCoord]->setWorld(this);
			chunks[chunkCoord]->initializeMesh();
		}
	}
}

void WorldManager::draw(Shader& ourShader)
{
	for (const auto& pair : chunks)
	{
		ChunkManager* chunk = pair.second;
		chunk->draw(ourShader);

	}
}

void WorldManager::update(const Camera& camera)
{

}

bool WorldManager::isBlockAir(const glm::ivec3& worldPos)
{
	int chunkX = worldPos.x / CHUNK_WIDTH;
	int chunkZ = worldPos.z / CHUNK_DEPTH;

	if (worldPos.x < 0)
	{
		chunkX--;
	}
	if (worldPos.z < 0)
	{
		chunkZ--;
	}

	ChunkCoord chunkCoord(chunkX, chunkZ);
	auto it = chunks.find(chunkCoord);
	if (it == chunks.end())
	{
		return true;
	}

	ChunkManager* chunk = it->second;

	glm::ivec3 localPos = {
		worldPos.x - chunkX * CHUNK_WIDTH,
		worldPos.y,
		worldPos.z - chunkZ * CHUNK_DEPTH
	};


	if (localPos.y < 0 || localPos.y >= CHUNK_HEIGHT)
	{
		return true;
	}

	return chunk->isBlockAir(localPos);
}