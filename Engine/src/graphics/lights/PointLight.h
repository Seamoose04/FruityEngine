#pragma once

#include "../Light.h"

class PointLight : public Light {
public:
	glm::vec3 position = glm::vec3(0.0f);
	float radius = 10.0f;
};
