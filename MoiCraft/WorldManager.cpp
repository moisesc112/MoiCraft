#include "WorldManager.h"

WorldManager::WorldManager()
{
	this->worldSeed = static_cast<unsigned int>(std::time(nullptr));
}

void WorldManager::initialize()
{

	chunkWorker = std::thread([this]() {
		while (!terminateWorker) {
			ChunkCoord coord;
			{
				std::unique_lock<std::mutex> lock(chunkQueueMutex);
				chunkQueueCV.wait(lock, [this]() {
					return !chunkLoadRequestQueue.empty() || terminateWorker;
					});

				if (terminateWorker) break;
				coord = chunkLoadRequestQueue.front();
				chunkLoadRequestQueue.pop();
			}

			ChunkManager* chunk = new ChunkManager(glm::ivec3(coord.x * CHUNK_WIDTH, 0, coord.z * CHUNK_DEPTH), worldSeed);

			{
				std::lock_guard<std::mutex> lock(chunkQueueMutex);
				readyChunks.push({ coord, chunk });
			}
		}
		});


	for (int i = -RENDER_DISTANCE; i <= RENDER_DISTANCE; i++)
	{
		for (int j = -RENDER_DISTANCE; j <= RENDER_DISTANCE; j++)
		{
			ChunkCoord coord(i, j);
			chunkLoadQueuedSet.insert(coord);

			{
				std::lock_guard<std::mutex> lock(chunkQueueMutex);
				chunkLoadRequestQueue.push(coord);
			}
			chunkQueueCV.notify_one(); 
		}
	}
}

void WorldManager::update(const Camera& camera, float deltaTime)
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

			if (chunks.find(chunkCoord) == chunks.end() &&
				chunkLoadQueuedSet.find(chunkCoord) == chunkLoadQueuedSet.end())
			{
				chunkLoadQueue.push(chunkCoord);
				chunkLoadQueuedSet.insert(chunkCoord);
			}
		}
	}

	chunkLoadTimer += deltaTime;

	if (chunkLoadTimer >= chunkLoadCooldown)
	{
		int loaded = 0;

		while (!chunkLoadQueue.empty() && loaded < chunksPerFrame)
		{
			ChunkCoord coord = chunkLoadQueue.front();
			chunkLoadQueue.pop();

			{
				std::lock_guard<std::mutex> lock(chunkQueueMutex);
				chunkLoadRequestQueue.push(coord);
			}

			chunkQueueCV.notify_one();
			loaded++;
		}

		chunkLoadTimer = 0.0f; 
	}

	{
		std::lock_guard<std::mutex> lock(chunkQueueMutex);
		while (!readyChunks.empty())
		{
			ChunkData data = readyChunks.front();
			readyChunks.pop();

			chunks[data.coord] = data.chunk;
			chunkLoadQueuedSet.erase(data.coord);
			chunkLoadStagedCurrent.push_back(data.coord);
		}
	}


	for (ChunkCoord& coord : chunkLoadStagedLast)
	{
		if (chunks.find(coord) != chunks.end()) {
			ChunkCoord neighbors[4] = {
			ChunkCoord(coord.x + 1, coord.z),
			ChunkCoord(coord.x - 1, coord.z),
			ChunkCoord(coord.x, coord.z + 1),
			ChunkCoord(coord.x, coord.z - 1)
			};

			bool allNeighborsExist = true;
			for (const auto& neighbor : neighbors)
			{
				if (chunks.find(neighbor) == chunks.end()) {
					allNeighborsExist = false;
					break;
				}
			}

			if (allNeighborsExist)
			{
				chunks[coord]->setWorld(this);
				chunks[coord]->initializeMesh();
			}
			else
			{
				chunkLoadStagedCurrent.push_back(coord);
			}
		}
	}

	chunkLoadStagedLast.clear();
	std::swap(chunkLoadStagedCurrent, chunkLoadStagedLast);
	chunkLoadStagedCurrent.clear();

	int remeshPerFrame = 1;
	if (!dirtyChunks.empty())
	{
		for (int i = 0; i < remeshPerFrame && !dirtyChunks.empty(); ++i) {
			ChunkCoord coord = dirtyChunks.front();
			dirtyChunks.erase(dirtyChunks.begin());

			auto it = chunks.find(coord);
			if (it != chunks.end()) {
				it->second->initializeMesh();
			}
		}
	}
}

void WorldManager::draw(Shader& shader)
{
	for (const auto& pair : chunks)
		pair.second->draw(shader);
}

void WorldManager::loadChunk(int x, int z)
{
	ChunkCoord chunkCoord(x, z);
	if (chunks.find(chunkCoord) == chunks.end())
	{
		chunks[chunkCoord] = new ChunkManager(glm::ivec3(x * CHUNK_WIDTH, 0, z * CHUNK_DEPTH), worldSeed);
		chunkLoadQueuedSet.erase(chunkCoord);
		chunkLoadStagedCurrent.push_back(chunkCoord);
		
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
				if (std::find(dirtyChunks.begin(), dirtyChunks.end(), neighbor) == dirtyChunks.end()) {
					dirtyChunks.push_back(neighbor);
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
			chunkLoadQueuedSet.erase(it->first);
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
	terminateWorker = true;
	chunkQueueCV.notify_all();
	if (chunkWorker.joinable())
		chunkWorker.join();


	for (auto& pair : chunks)
		delete pair.second;
	
	chunks.clear();
}