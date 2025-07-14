#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include "Camera.h"

class InputManager
{
public:
	InputManager();
	void processInput(GLFWwindow* window, Camera& camera);
private:

};
#endif