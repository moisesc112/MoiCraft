#include "WorldManager.h"

WorldManager::WorldManager()
{
	loadChunk(0, 0);
	loadChunk(1, 0);
	loadChunk(1, 1);
}


void WorldManager::loadChunk(float x, float z)
{
	ChunkCoord chunkCoord(x, z);
	if (chunks.find(chunkCoord) == chunks.end())
	{
		chunks[chunkCoord] = new ChunkManager(glm::vec3(x * CHUNK_WIDTH, 0.0f, z * CHUNK_DEPTH));
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