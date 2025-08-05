# MoiCraft

A Minecraft-style voxel engine written in C++ using OpenGL. This project focuses on procedural terrain generation, chunk-based rendering, and performance optimization using multi-threaded chunk loading.

## Features

- Written using modern OpenGL and C++
- Infinite procedurally generated terrain using Perlin noise
- Chunk-based world management
- Multi-threaded chunk generation to reduce frame lag
- First-person camera controls
- Optimized mesh generation with face culling

## Technologies Used

- C++
- OpenGL (via GLAD)
- GLFW for windowing and input
- GLM for math
- stb_image for texture loading
- Custom Perlin noise for terrain generation

## Project Structure

- Block.* # Block data and face generation
- Camera.* # First-person camera and view matrix logic
- ChunkManager.* # Handles block data and mesh generation for a chunk
- Config.h # Constants (e.g., chunk size, render distance)
- Game.* # Main game loop and update/render logic
- InputManager.* # Handles keyboard and mouse input
- TextureManager.* # Texture loading and management
- Window.* # GLFW window wrapper
- WorldManager.* # Manages visible chunks and background loading
- main.cpp # Program entry point

## Multi-Threading

Chunks are generated in a background thread using a job queue. This helps reduce frame drops when loading new terrain.

## Controls

- `W A S D` — Move
- 'SPACE' — Move Upwards
- 'LSHIFT' — Move Downwards
- Mouse — Look around
- `ESC` — Exit

## Screenshots

![Generated Terrain](MoiCraft\docs\terrain.png)

![Interior face culling](MoiCraft\docs\interior.png)

## Build Instructions

### Requirements

- C++17 or newer
- CMake (or build using Visual Studio)
- OpenGL 3.3+
- GLFW
- GLAD
- stb_image

### Windows (Visual Studio)

1. Clone the repo:
   ```bash
   git clone https://github.com/yourusername/moicraft.git
   cd moicraft
2. Open the .sln file in Visual Studio 2022.
3. Set the build mode to Release.
4. Build and run.