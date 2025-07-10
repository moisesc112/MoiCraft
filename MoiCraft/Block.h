#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>
#include <shader_s.h>
#include <vector>

class Block
{
private:
	glm::vec3 position;
	std::vector<float> vertices;

public:
	
	Block(glm::vec3 position);
	void setVertices();
	std::vector<float> getVertices();
	void draw(Shader& shader, unsigned int VAO);

};

#endif

