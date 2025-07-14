#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(unsigned int screenWidth, unsigned int screenHeight);
	void processKeyboard(char keyPress);

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::mat4 view;
	glm::mat4 projection;
	float speed;
};

#endif 