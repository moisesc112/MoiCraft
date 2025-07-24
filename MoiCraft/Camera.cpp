#include "Camera.h"
#include <iostream>

Camera* Camera::instance = nullptr;

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed, unsigned int screenWidth, unsigned int screenHeight)
{
	this->position = position;
	this->front = front;
	this->up = up;
	this->speed = speed;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(70.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}

void Camera::setInstance(Camera* camera)
{
	instance = camera;
}

Camera* Camera::getInstance()
{
	return instance;
}

glm::mat4 Camera::getViewMatrix()
{
	return view = glm::lookAt(position, position + front, up);;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projection;
}

glm::vec3 Camera::getPosition() const
{
	return position;
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
		case '_':
			position += speed * up;
			break;
		case '-':
			position -= speed * up;
			break;
	}
}


void Camera::cursorCallback(GLFWwindow* window, double mouseX, double mouseY)
{
	if (instance)
	{
		instance->processCursor(mouseX, mouseY);
	}
}

void Camera::processCursor(double mouseX, double mouseY)
{
	double centerX = screenWidth / 2.0;
	double centerY = screenHeight / 2.0f;

	double offsetX = mouseX - centerX;
	double offsetY = mouseY- centerY;

	glm::vec2 currentMouse = glm::vec2(offsetX, offsetY);

	static bool firstLook = true;
	if (firstLook)
	{
		oldMousePosition = currentMouse;
		firstLook = false;
	}

	glm::vec2 mouseDelta = oldMousePosition - currentMouse;

	float mouseSensitivity = 0.5f;

	yaw = mouseDelta.x * mouseSensitivity;
	pitch = mouseDelta.y * mouseSensitivity;

	//std::cout << "mouseX: " << mouseX << std::endl;
	std::cout << "pitch: " << pitch << std::endl;

	front = glm::normalize(glm::rotate(front, (float)glm::radians(yaw), up));
    front = glm::normalize(glm::rotate(front, (float)glm::radians(pitch), glm::normalize(glm::cross(front, up))));

	oldMousePosition = currentMouse;
}

void Camera::updateProjection(int width, int height)
{
	projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.1f, 100.0f);
}