#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <shader_s.h>
#include <vector>

class Block
{
public:

	Block();
	Block(glm::vec3 position);
	void setVertices(std::vector<float> vertices);
	std::vector<float> getVertices();
	void draw(Shader& shader, glm::vec3 offset, unsigned int VAO);

private:
	glm::vec3 position;
	std::vector<float> vertices;

};

#endif

