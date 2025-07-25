#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <shader_s.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Window.h"
#include "Block.h"
#include "Camera.h"
#include "InputManager.h"
#include "ChunkManager.h"
#include "WorldManager.h"
#include "TextureManager.h"
#include <windows.h>
#include <psapi.h>

// settings
const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;
glm::vec3 cameraPos = glm::vec3(0.0f, 25.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 
const float cameraSpeed = 0.05f;

size_t getMemoryUsageMB() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024 * 1024); // in MB
    }
    return 0;
}

int main()
{

    Window window(screenWidth, screenHeight, "MoiCraft");
    Camera camera(cameraPos, cameraFront, cameraUp, cameraSpeed, screenWidth, screenHeight);
    InputManager inputManager;
    Block block;

    glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Camera::setInstance(&camera);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ChunkManager chunkManager;
    WorldManager worldManager;
    TextureManager textureManager;

    glEnable(GL_DEPTH_TEST);

    Shader ourShader(
        "C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/shader.vs",
        "C:/Users/moise/Documents/VS_projects/MoiCraft/MoiCraft/includes/shader.fs"
    );

    textureManager.AddTextures();

    while (!window.shouldClose())
    {
        inputManager.processInput(window.getGLFWwindow(), camera);
        glfwSetCursorPosCallback(window.getGLFWwindow(), Camera::cursorCallback);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        ourShader.use();
        textureManager.ActivateTexture(ourShader);

        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

        ourShader.setMat4("projection", camera.getProjectionMatrix());
        ourShader.setMat4("view", camera.getViewMatrix());

        glBindVertexArray(chunkManager.getVAO());
        
        worldManager.update(camera);
        worldManager.draw(ourShader);
        std::cout << "Memory usage: " << getMemoryUsageMB() << " MB\n";

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}

