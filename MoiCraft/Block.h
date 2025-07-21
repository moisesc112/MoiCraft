#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <shader_s.h>
#include <vector>

enum class BlockType
{
	Air,
	Grass,
	Dirt,
	Stone
};

enum class FaceDirection
{
	Front,
	Back,
	Left,
	Right,
	Bottom,
	Top
};

class Block
{
public:

	Block();
	Block(glm::ivec3 position);
	void setVertices(std::vector<float> vertices);
	const std::vector<std::vector<float>> getFaceData();
	bool isAir();
	BlockType getBlockType();
	void setBlockType(BlockType blockType);

private:
	glm::ivec3 position;
	std::vector<float> vertices;
	//std::vector<std::vector<float>> faceData;
	BlockType blockType;

};

#endif

