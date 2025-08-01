#include "WorldManager.h"

WorldManager::WorldManager()
{
	this->worldSeed = static_cast<unsigned int>(std::time(nullptr));
}

void WorldManager::initialize()
{
	for (int i = -RENDER_DISTANCE; i <= RENDER_DISTANCE; i++)
	{
		for (int j = -RENDER_DISTANCE; j <= RENDER_DISTANCE; j++)
		{
			loadChunk(i, j, false);
		}
	}
	
	for (auto& pair : chunks)
	{
		pair.second->setWorld(this);
		pair.second->initializeMesh();
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
			ChunkCoord chunkCoord(chunkPosX, chunkPosZ);
			loadChunk(chunkPosX, chunkPosZ, false);
		}
	}

	for (auto& pair : chunks)
	{
		pair.second->setWorld(this);
		pair.second->initializeMesh();
	}
}

void WorldManager::draw(Shader& shader)
{
	for (const auto& pair : chunks)
		pair.second->draw(shader);
}

void WorldManager::loadChunk(int x, int z, bool initialize)
{
	ChunkCoord chunkCoord(x, z);
	if (chunks.find(chunkCoord) == chunks.end())
	{
		chunks[chunkCoord] = new ChunkManager(glm::ivec3(x * CHUNK_WIDTH, 0, z * CHUNK_DEPTH), worldSeed);
		chunks[chunkCoord]->setWorld(this);
		if (initialize)
			chunks[chunkCoord]->initializeMesh();
		else
		{
			ChunkCoord neighbors[4] = {
			ChunkCoord(x + 1, z),
			ChunkCoord(x - 1, z),
			ChunkCoord(x, z + 1),
			ChunkCoord(x, z - 1)
			};

			for (const auto& neighbor : neighbors)
			{
				auto it = chunks.find(neighbor);
				if (it != chunks.end()) {
					it->second->markDirty();
				}
			}
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
			++it;
	}
}

bool WorldManager::isBlockAir(const glm::ivec3& worldPos)
{
	int chunkX = static_cast<int>(std::floor(worldPos.x / static_cast<float>(CHUNK_WIDTH)));
	int chunkZ = static_cast<int>(std::floor(worldPos.z / static_cast<float>(CHUNK_DEPTH)));

	ChunkCoord chunkCoord(chunkX, chunkZ);
	auto it = chunks.find(chunkCoord);
	if (it == chunks.end())
		return true;

	ChunkManager* chunk = it->second;

	glm::ivec3 localPos = {
		worldPos.x - chunkX * CHUNK_WIDTH,
		worldPos.y,
		worldPos.z - chunkZ * CHUNK_DEPTH
	};


	if (localPos.y < 0 || localPos.y >= CHUNK_HEIGHT)
		return true;

	return chunk->isBlockAir(localPos);
}

WorldManager::~WorldManager()
{
	for (auto& pair : chunks)
		delete pair.second;
	
	chunks.clear();
}