cmake_minimum_required(VERSION 3.20)
project(MoiCraft)
set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "-O3")
elseif (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "-g")
endif()

file(GLOB_RECURSE MoiCraftSources CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/MoiCraft/src/*.cpp
)
add_executable(MoiCraft ${MoiCraftSources})

add_subdirectory(third_party/glfw EXCLUDE_FROM_ALL)

add_library(glad ${CMAKE_SOURCE_DIR}/MoiCraft/includes/glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/MoiCraft/includes)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE
  ${CMAKE_SOURCE_DIR}/MoiCraft/includes/stb
)

target_include_directories(MoiCraft PRIVATE
  ${CMAKE_SOURCE_DIR}/MoiCraft/src
  ${CMAKE_SOURCE_DIR}/MoiCraft/src/core
  ${CMAKE_SOURCE_DIR}/MoiCraft/src/window
  ${CMAKE_SOURCE_DIR}/MoiCraft/src/camera
  ${CMAKE_SOURCE_DIR}/MoiCraft/src/input
  ${CMAKE_SOURCE_DIR}/MoiCraft/src/rendering
  ${CMAKE_SOURCE_DIR}/MoiCraft/src/world
  ${CMAKE_SOURCE_DIR}/MoiCraft/includes
  ${CMAKE_SOURCE_DIR}/MoiCraft/includes/glad
  ${CMAKE_SOURCE_DIR}/MoiCraft/includes/glm
  ${CMAKE_SOURCE_DIR}/MoiCraft/includes/stb
  ${CMAKE_SOURCE_DIR}/MoiCraft/includes/perlin\ noise
  ${CMAKE_SOURCE_DIR}/MoiCraft/shaders
)

find_package(OpenGL REQUIRED)
target_link_libraries(MoiCraft PUBLIC glfw glad stb OpenGL::GL)

if (WIN32)
  add_custom_command(
    TARGET MoiCraft PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/MoiCraft/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/MoiCraft/shaders
            ${CMAKE_CURRENT_BINARY_DIR}/shaders
  )
else()
  add_custom_command(
    TARGET MoiCraft PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/MoiCraft/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/MoiCraft/shaders
            ${CMAKE_CURRENT_BINARY_DIR}/shaders
  )
endif()