#include "ChunkManager.h"
#include "WorldManager.h"

ChunkManager::ChunkManager()
{

}

ChunkManager::ChunkManager(glm::ivec3 position, unsigned int seed)
{
	this->chunkPosition = position;
	
	faceData = block.getFaceData();

	siv::PerlinNoise noiseGenerator(seed);

	blocks.resize(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH);

	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
		for (int j = 0; j < CHUNK_HEIGHT; ++j)
		{
			for (int k = 0; k < CHUNK_DEPTH; ++k)
			{			

				float worldPosX = chunkPosition.x + i;
				float worldPosZ = chunkPosition.z + k;

				float base = noiseGenerator.noise2D_01(worldPosX * 0.01f, worldPosZ * 0.01f) * 0.05f;
				float mountain = std::pow(noiseGenerator.noise2D_01(worldPosX * 0.05f, worldPosZ * 0.05f), 1.5f) * 30.0f;
				float height = base + mountain;

				if (j <= height) {
					if (j >= 4 && j < 7) 
					{
						getBlock(i, j, k).setBlockType(BlockType::Dirt);
					}
					else if (j >= 7 && j < 10)
					{
						getBlock(i, j, k).setBlockType(BlockType::Grass);
					}
					else
					{
						getBlock(i, j, k).setBlockType(BlockType::Stone);
					}
				}
				else {
					getBlock(i, j, k).setBlockType(BlockType::Air);
				}
			}
		}
	}
}

void ChunkManager::setWorld(WorldManager* worldManager)
{
	this->world = worldManager;
}

void ChunkManager::initializeMesh()
{

	if (meshInitialized && !dirty)
	{
		return;
	}

	if (!world) {
		std::cerr << "World is null during mesh generation!\n";
	}

	generateMesh();
	if (!meshInitialized)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		meshInitialized = true;
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// faceID attribute
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// blockType attribute
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	dirty = false;
}

void ChunkManager::generateMesh()
{
	vertices.clear();

	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
		for (int j = 0; j < CHUNK_HEIGHT; ++j) 
		{
			for (int k = 0; k < CHUNK_DEPTH; ++k)
			{
				glm::ivec3 position = { i,j,k };

				if (getBlock(i, j, k).isAir())
					continue;

				if (isFaceVisible(position, { 0, 0, -1 })) 
				{
					addFace(faceData[static_cast<int>(FaceDirection::Front)], position);
				}
				if (isFaceVisible(position, { 0, 0, 1 }))
				{
					addFace(faceData[static_cast<int>(FaceDirection::Back)], position);
				}
				if (isFaceVisible(position, { -1, 0, 0 }))
				{
					addFace(faceData[static_cast<int>(FaceDirection::Left)], position);
				}
				if (isFaceVisible(position, { 1, 0, 0 }))
				{
					addFace(faceData[static_cast<int>(FaceDirection::Right)], position);
				}
				if (isFaceVisible(position, { 0, -1, 0 }))
				{
					addFace(faceData[static_cast<int>(FaceDirection::Bottom)], position);
				}
				if (isFaceVisible(position, { 0, 1, 0 }))
				{
					addFace(faceData[static_cast<int>(FaceDirection::Top)], position);
				}
			}
		}
	}
}

void ChunkManager::draw(Shader& ourShader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(chunkPosition));
	ourShader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 7);
}


unsigned int ChunkManager::getVAO()
{
	return VAO;
}

unsigned int ChunkManager::getVBO()
{
	return VBO;
}

void ChunkManager::deleteVAO()
{
	glDeleteVertexArrays(1, &VAO);
}

void ChunkManager::deleteVBO()
{
	glDeleteBuffers(1, &VBO);
}

void ChunkManager::addFace(const std::vector<float>& faceData, const glm::ivec3& position)
{
	float blockType = static_cast<float>(getBlock(position.x, position.y, position.z).getBlockType());

	for (int i = 0; i < faceData.size(); i += 7)
	{
		float x = faceData[i + 0] + position.x;
		float y = faceData[i + 1] + position.y;
		float z = faceData[i + 2] + position.z;
		float u = faceData[i + 3];
		float v = faceData[i + 4];
		float faceID = faceData[i + 5];

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		vertices.push_back(u);
		vertices.push_back(v);
		vertices.push_back(faceID);
		vertices.push_back(blockType);
	}
}

bool ChunkManager::isFaceVisible(const glm::ivec3& position, const glm::ivec3& direction)
{
	glm::ivec3 neighborPos = position + direction;

	if (neighborPos.x >= 0 && neighborPos.x < CHUNK_WIDTH &&
		neighborPos.y >= 0 && neighborPos.y < CHUNK_HEIGHT &&
		neighborPos.z >= 0 && neighborPos.z < CHUNK_DEPTH)
	{
		return getBlock(neighborPos.x, neighborPos.y, neighborPos.z).isAir();
	}
	else
	{
		glm::ivec3 worldBlockPos = chunkPosition + position;
		glm::ivec3 worldNeighborPos = worldBlockPos + direction;
		return world->isBlockAir(worldNeighborPos);
	}

}

bool ChunkManager::isBlockAir(const glm::ivec3& localPos)
{

	if (localPos.x < 0 || localPos.x >= CHUNK_WIDTH ||
		localPos.y < 0 || localPos.y >= CHUNK_HEIGHT ||
		localPos.z < 0 || localPos.z >= CHUNK_DEPTH)
	{
		return true;
	}
	
	return getBlock(localPos.x, localPos.y, localPos.z).isAir();
}

void ChunkManager::markDirty() {
	dirty = true;
}

inline Block& ChunkManager::getBlock(int x, int y, int z) 
{
	return blocks[x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z)];
}

ChunkManager::~ChunkManager()
{
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	if (VBO != 0)
		glDeleteBuffers(1, &VBO);
}