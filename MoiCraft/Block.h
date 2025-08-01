#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
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

	const std::vector<std::vector<float>> getFaceData();

	bool isAir();
	BlockType getBlockType() const;
	void setBlockType(BlockType blockType);

private:
	glm::ivec3 position;
	BlockType blockType;
};

#endif

