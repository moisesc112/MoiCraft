#include "ChunkManager.h"

ChunkManager::ChunkManager()
{

}

ChunkManager::ChunkManager(glm::vec3 position)
{
	this->chunkPosition = position;
	
	//std::vector<float> vertices = block.getVertices();
	std::vector<std::vector<float>> faceData = block.getFaceData();

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

	generateMesh();

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
}


void ChunkManager::generateMesh()
{
	std::vector<std::vector<float>> faceData = block.getFaceData();

	for (int i = 0; i < CHUNK_WIDTH; ++i)
	{
		for (int j = 0; j < CHUNK_HEIGHT; ++j)
		{
			for (int k = 0; k < CHUNK_DEPTH; ++k)
			{
				if (i == CHUNK_WIDTH / 2 || i == CHUNK_HEIGHT / 2 || i == CHUNK_DEPTH / 2)
					blocks[i][j][k].setBlockType(BlockType::Air);

				if (blocks[i][j][k].isAir())
					continue;
				//blocks[i][j][k] = Block(glm::vec3(i, j, k));

				//if (isFaceVisible(i, j, k, { 1, 0, 0 }))
				//{
					//addFace(block.returnFaceVertices('R'), glm::vec3(i, j, k));
				//}
				if (isFaceVisible(i, j, k, {0, 0, -1}))
					addFace(faceData[0], glm::vec3(i, j, k));
				if (isFaceVisible(i, j, k, {0, 0, 1}))
					addFace(faceData[1], glm::vec3(i, j, k));
				if (isFaceVisible(i, j, k, {-1, 0, 0}))
					addFace(faceData[2], glm::vec3(i, j, k));
				if (isFaceVisible(i, j, k, {1, 0, 0}))
					addFace(faceData[3], glm::vec3(i, j, k));
				if (isFaceVisible(i, j, k, {0, -1, 0}))
					addFace(faceData[4], glm::vec3(i, j, k));
				if (isFaceVisible(i, j, k, {0, 1, 0}))
					addFace(faceData[5], glm::vec3(i, j, k));

			}
		}
	}


}

void ChunkManager::draw(Shader& ourShader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, chunkPosition);
	ourShader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 5);
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

void ChunkManager::addFace(std::vector<float> faceData, glm::vec3 position)
{
	for (int i = 0; i < faceData.size(); i += 5)
	{
		float x = faceData[i + 0] + position.x;
		float y = faceData[i + 1] + position.y;
		float z = faceData[i + 2] + position.z;
		float u = faceData[i + 3];
		float v = faceData[i + 4];

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		vertices.push_back(u);
		vertices.push_back(v);
	}
}

bool ChunkManager::isFaceVisible(int x, int y, int z, glm::vec3 direction)
{
	int nx = x + direction.x;
	int ny = y + direction.y;
	int nz = z + direction.z;

	// Out of bounds = visible face (since it's at chunk edge)
	if ((nx < 0 || nx >= CHUNK_WIDTH) || (ny < 0 || ny >= CHUNK_HEIGHT) || (nz < 0 || nz >= CHUNK_DEPTH))
	{
		return true;
	}
	
	//return false;
	return blocks[nx][ny][nz].isAir();
}