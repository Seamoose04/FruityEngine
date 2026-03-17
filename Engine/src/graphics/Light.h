#pragma once

#include <glm/glm.hpp>

class Light {
public:
	glm::vec3 color = glm::vec3(0.0f);
	float intensity = 0.0f;

	virtual ~Light() = default;
};
