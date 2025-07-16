#include "ChunkManager.h"

ChunkManager::ChunkManager()
{

}

ChunkManager::ChunkManager(glm::vec3 position)
{
	this->chunkPosition = position;

	Block block;

	std::vector<float> vertices = block.getVertices();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
		for (int j = 0; j < CHUNK_HEIGHT; ++j)
		{
			for (int k = 0; k < CHUNK_DEPTH; ++k)
			{
				blocks[i][j][k] = Block(glm::vec3(i, j, k));
			}
		}
	}
}

void ChunkManager::draw(Shader& ourShader)
{
	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
		for (int j = 0; j < CHUNK_HEIGHT; ++j)
		{
			for (int k = 0; k < CHUNK_DEPTH; ++k)
			{
				blocks[i][j][k].draw(ourShader, chunkPosition, VAO);
			}
		}
	}
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