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

	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
		for (int j = 0; j < CHUNK_HEIGHT; ++j)
		{
			for (int k = 0; k < CHUNK_DEPTH; ++k)
			{
				blocks[i][j][k] = Block(glm::ivec3(i, j, k));

				float worldPosX = chunkPosition.x + i;
				float worldPosZ = chunkPosition.z + k;
				//float height = noiseGenerator.noise2D_01(worldPosX * 0.05f, worldPosZ * 0.05f) * (CHUNK_HEIGHT * 3.0f);

				//int height = static_cast<int>(std::round(noiseGenerator.noise2D_01(worldPosX * 0.05f, worldPosZ * 0.05f) * (CHUNK_HEIGHT * 30.0f)));
				float height = std::pow(noiseGenerator.noise2D_01(worldPosX * 0.15f, worldPosZ * 0.15f), 5.0f) * (CHUNK_HEIGHT * 6.0f);
				if (j <= height) {
					if (j >= 4 && j < 7) 
					{
						blocks[i][j][k].setBlockType(BlockType::Dirt);
					}
					else if (j >= 7 && j < 10)
					{
						blocks[i][j][k].setBlockType(BlockType::Grass);
					}
					else
					{
						blocks[i][j][k].setBlockType(BlockType::Stone);
					}
				}
				else {
					blocks[i][j][k].setBlockType(BlockType::Air);
				}


				//if (i == CHUNK_WIDTH / 2 || j == CHUNK_HEIGHT / 2 || k == CHUNK_DEPTH / 2)
				//{
					//blocks[i][j][k].setBlockType(BlockType::Air);
				//}
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
	if (!world) {
		std::cerr << "World is null during mesh generation!\n";
	}
	generateMesh();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

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
}

void ChunkManager::generateMesh()
{
	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
		for (int j = 0; j < CHUNK_HEIGHT; ++j) 
		{
			for (int k = 0; k < CHUNK_DEPTH; ++k)
			{
				glm::ivec3 position = { i,j,k };
				/*
				float worldPosX = chunkPosition.x + i;
				float worldPosZ = chunkPosition.z + k;
				//float height = noiseGenerator.noise2D_01(worldPosX * 0.05, worldPosZ * 0.05) * CHUNK_HEIGHT;

				int terrainHeight = static_cast<int>(std::round(
					noiseGenerator.noise2D_01(worldPosX * 0.05f, worldPosZ * 0.05f) * (CHUNK_HEIGHT - 1)
				));

				if (j <= terrainHeight) {
					blocks[i][j][k].setBlockType(BlockType::Stone);
				}
				else {
					blocks[i][j][k].setBlockType(BlockType::Air);
				}
				*/
				if (blocks[i][j][k].isAir())
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
	float blockType = static_cast<float>(blocks[position.x][position.y][position.z].getBlockType());

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
		return blocks[neighborPos.x][neighborPos.y][neighborPos.z].isAir();
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
	
	return blocks[localPos.x][localPos.y][localPos.z].isAir();
}