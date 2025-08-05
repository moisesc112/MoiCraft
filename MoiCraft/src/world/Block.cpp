#include "Block.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::vector<float> FACE_FRONT = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 5.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 5.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 5.0f, 0.0f
};


std::vector<float> FACE_BACK = {
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 4.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 4.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 4.0f, 0.0f
};

std::vector<float> FACE_LEFT = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 2.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 2.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 2.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 2.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 2.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 2.0f, 0.0f
};

std::vector<float> FACE_RIGHT = {
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 3.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 3.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 3.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 3.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 3.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 3.0f, 0.0f
};

std::vector<float> FACE_BOTTOM = {
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f
};

std::vector<float> FACE_TOP = {
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f
};


const std::vector<std::vector<float>> GLOBAL_FACE_DATA = {
    FACE_FRONT,
    FACE_BACK,
    FACE_LEFT,
    FACE_RIGHT,
    FACE_BOTTOM,
    FACE_TOP
};

Block::Block()
{
    blockType = BlockType::Stone; 
}

Block::Block(glm::ivec3 position)
{
    this->position = position;
    blockType = BlockType::Stone;
}

const std::vector<std::vector<float>> Block::getFaceData()
{
    return GLOBAL_FACE_DATA;
}

bool Block::isAir()
{
    if (blockType == BlockType::Air) {
        return true;
    }
    else
    {
        return false; 
    } 
}

BlockType Block::getBlockType() const
{
    return blockType;
}

void Block::setBlockType(BlockType blockType)
{
    this->blockType = blockType;
}