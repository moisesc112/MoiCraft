#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Camera.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "WorldManager.h"
#include "Config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <shader_s.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


class Game
{
public:
	Game();
	~Game();
	void run();

private:
	void initialize();
	void processInput();
	void update();
	void render();

	Window window;
	Camera camera;
	InputManager inputManager;
	TextureManager textureManager;
	WorldManager worldManager;
	Shader* shader;
};

#endif 