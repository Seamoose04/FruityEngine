#pragma once

#include "game/Field.h"

#include <glm/glm.hpp>

class Vec3Field : public Field {
public:
	void FromJSON(const json &data) override;
	
	glm::vec3 value;
};
