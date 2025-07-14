#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed)
{
	this->position = position;
	this->front = front;
	this->up = up;
	this->speed = speed;
}

glm::mat4 Camera::getViewMatrix()
{
	return view = glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(unsigned int screenWidth, unsigned int screenHeight)
{
	projection = glm::mat4(1.0f);
	return projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}

void Camera::processKeyboard(char keyPress)
{
	switch (keyPress)
	{
		case 'W':
			position += speed * front;
			break;
		case 'A':
			position -= glm::normalize(glm::cross(front, up)) * speed;
			break;
		case 'S':
			position -= speed * front;
			break;
		case 'D':
			position += glm::normalize(glm::cross(front, up)) * speed;
			break;
	}
}