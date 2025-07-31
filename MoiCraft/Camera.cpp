#include "Camera.h"
#include <iostream>

Camera* Camera::instance = nullptr;

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed, float yaw, float pitch, float fov, unsigned int screenWidth, unsigned int screenHeight)
{
	this->position = position;
	this->up = up;
	this->speed = speed;
	this->yaw = yaw;
	this->pitch = pitch;
	this->fov = fov;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->front = calculateDirection(yaw, pitch);

	view = glm::mat4(1.0f);
	projection = glm::perspective(fov, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}

void Camera::setInstance(Camera* camera)
{
	instance = camera;
}

Camera* Camera::getInstance()
{
	return instance;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);;
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return projection;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::calculateDirection(float yaw, float pitch)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	return glm::normalize(direction);
}

void Camera::processKeyboard(char keyPress)
{
	glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 right = glm::normalize(glm::cross(front, up));

	switch (keyPress)
	{
		case 'W':
			position += speed * horizontalFront;
			break;
		case 'A':
			position -= right * speed;
			break;
		case 'S':
			position -= speed * horizontalFront;
			break;
		case 'D':
			position += right * speed;
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
	if (!instance)
	{
		return;
	}

	if (instance->firstLook)
	{
		instance->oldMousePosition = glm::vec2(mouseX, mouseY);
		instance->firstLook = false;
		return; 
	}

	instance->processCursor(mouseX, mouseY);
	
}

void Camera::processCursor(double mouseX, double mouseY)
{
	glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);
	glm::vec2 mouseDelta = oldMousePosition - currentMouse;
	oldMousePosition = currentMouse;

	float mouseSensitivity = 0.25f;
	yaw -= mouseDelta.x * mouseSensitivity;
	pitch += mouseDelta.y * mouseSensitivity;
	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	front = calculateDirection(yaw, pitch);
}

void Camera::updateProjection(int width, int height)
{
	projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 100.0f);
}