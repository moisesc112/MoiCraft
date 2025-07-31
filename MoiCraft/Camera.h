#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed, float yaw, float pitch, float fov, unsigned int screenWidth, unsigned int screenHeight);

	glm::mat4 getViewMatrix() const; 
	glm::mat4 getProjectionMatrix() const;
	glm::vec3 getPosition() const;

	glm::vec3 calculateDirection(float yaw, float pitch);

	void processKeyboard(char keyPress);
	void processCursor(double mouseX, double mouseY);
	void updateProjection(int width, int height);

	static void cursorCallback(GLFWwindow* window, double mouseX, double mouseY);
	static void setInstance(Camera* camera);

	static Camera* getInstance();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec2 oldMousePosition;

	glm::mat4 view;
	glm::mat4 projection;

	float speed;
	float yaw;
	float pitch;
	float fov;

	unsigned int screenWidth;
	unsigned int screenHeight;

	bool firstLook = true;

	static Camera* instance;
};

#endif 