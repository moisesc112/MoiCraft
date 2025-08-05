#pragma once
#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Camera.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "WorldManager.h"
#include "Config.h"
#include "shader_s.h"

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

private:
	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	float totalTime = 0.0f;
	int frameCount = 0;

	Window window;
	Camera camera;
	InputManager inputManager;
	TextureManager textureManager;
	WorldManager worldManager;
	Shader* shader;
};

#endif 