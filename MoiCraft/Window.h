#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window
{
public:
	Window(unsigned int width, unsigned int height, const char* title);
	~Window();

	void swapBuffers();
	void pollEvents();
	bool shouldClose() const;

	GLFWwindow* getGLFWwindow();

private:
	GLFWwindow* window;
	unsigned int width;
	unsigned int height;
	const char* title;

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif