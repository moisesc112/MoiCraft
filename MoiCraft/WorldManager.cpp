#include "WorldManager.h"

WorldManager::WorldManager()
{
	unsigned int worldSeed = static_cast<unsigned int>(std::time(nullptr));

	//loadChunk(0, 0, false);
	//loadChunk(1, 0, false);
	//loadChunk(1, 1, false);

	for (int i = 0; i < RENDER_DISTANCE; i++)
	{
		for (int j = 0; j < RENDER_DISTANCE; j++)
		{
			loadChunk(i, j, false);
		}
	}

	for(auto it = chunks.begin(); it != chunks.end(); ++it) 
	{
		ChunkManager* chunk = it->second;
		chunk->setWorld(this);
	}

	for (auto it = chunks.begin(); it != chunks.end(); ++it) 
	{
		ChunkManager* chunk = it->second;
		chunk->initializeMesh();
	}
}


void WorldManager::loadChunk(float x, float z, bool initialize)
{
	ChunkCoord chunkCoord(x, z);
	if (chunks.find(chunkCoord) == chunks.end())
	{
		chunks[chunkCoord] = new ChunkManager(glm::ivec3(x * CHUNK_WIDTH, 0.0f, z * CHUNK_DEPTH), worldSeed);
		if (initialize)
		{
			chunks[chunkCoord]->setWorld(this);
			chunks[chunkCoord]->initializeMesh();
		}
	}
}

void WorldManager::unloadChunk(int playerChunkPosX, int playerChunkPosZ)
{
	for (auto it = chunks.begin(); it != chunks.end();)
	{
		int dx = it->first.x - playerChunkPosX;
		int dz = it->first.z - playerChunkPosZ;

		if (std::abs(dx) > RENDER_DISTANCE || std::abs(dz) > RENDER_DISTANCE)
		{
			delete it->second;
			it = chunks.erase(it);
		}
		else
		{
			++it;
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
	glm::vec3 playerPos = camera.getPosition();
	
	int playerChunkPosX = static_cast<int>(std::floor(playerPos.x / CHUNK_WIDTH));
	int playerChunkPosZ = static_cast<int>(std::floor(playerPos.z / CHUNK_DEPTH));


	unloadChunk(playerChunkPosX, playerChunkPosZ);

	for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; ++dx)
	{
		for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; ++dz)
		{
			int chunkPosX = playerChunkPosX + dx;
			int chunkPosZ = playerChunkPosZ + dz;
			loadChunk(chunkPosX, chunkPosZ, true);
		}
	}

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