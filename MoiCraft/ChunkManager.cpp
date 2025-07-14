#include "ChunkManager.h"

ChunkManager::ChunkManager()
{
	for (int i = 0; i < chunkWidth; ++i)
	{
		for (int j = 0; j < chunkHeight; ++j)
		{
			for (int k = 0; k < chunkDepth; ++k)
			{
				blocks[i][j][k] = Block(glm::vec3(i, j, k));
			}
		}
	}
}

void ChunkManager::draw(Shader& ourShader, unsigned int VAO)
{
	for (int i = 0; i < chunkWidth; ++i)
	{
		for (int j = 0; j < chunkHeight; ++j)
		{
			for (int k = 0; k < chunkDepth; ++k)
			{
				blocks[i][j][k].draw(ourShader, VAO);
			}
		}
	}
}