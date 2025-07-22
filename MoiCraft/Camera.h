#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed, unsigned int screenWidth, unsigned int screenHeight);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getPosition() const;
	void processKeyboard(char keyPress);
	void processCursor(double mouseX, double mouseY);

	static void cursorCallback(GLFWwindow* window, double mouseX, double mouseY);
	static void setInstance(Camera* camera);

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec2 oldMousePosition;
	glm::vec3 direction;
	float speed;
	float yaw;
	float pitch;
	unsigned int screenWidth;
	unsigned int screenHeight;

	static Camera* instance;
};

#endif 