#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Game::Game()
    : window(Config::screenWidth, Config::screenHeight, Config::screenTitle),
    camera(Config::cameraPos, Config::cameraFront, Config::cameraUp, Config::cameraSpeed, Config::yaw, Config::pitch, Config::fov, Config::screenWidth, Config::screenHeight)
{
    initialize();
}


void Game::initialize()
{
    glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Camera::setInstance(&camera);

    glfwSetCursorPosCallback(window.getGLFWwindow(), Camera::cursorCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    shader = new Shader("C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/shader.vs", "C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/shader.fs");

    worldManager.initialize();
    textureManager.AddTextures();
}

void Game::run()
{
    while (!window.shouldClose()) 
    {
        processInput();
        update();
        render();

        window.swapBuffers();
        window.pollEvents();
    }
}

void Game::processInput()
{
    inputManager.processInput(window.getGLFWwindow(), camera);
}

void Game::update()
{
    worldManager.update(camera);
}

void Game::render()
{
    window.clearColor();

    shader->use();
    textureManager.ActivateTexture(*shader);

    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader->ID, "view");

    shader->setMat4("projection", camera.getProjectionMatrix());
    shader->setMat4("view", camera.getViewMatrix());

    worldManager.draw(*shader);
}

Game::~Game()
{
    delete shader;
}