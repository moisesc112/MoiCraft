#pragma once
//#include <glm/glm.hpp>

namespace Config {
	inline constexpr unsigned int screenWidth = 800;
	inline constexpr unsigned int screenHeight = 600;

	inline const glm::vec3 cameraPos = glm::vec3(0.0f, 25.0f, 5.0f);
	inline const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	inline const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	inline constexpr float cameraSpeed = 0.05f;
}