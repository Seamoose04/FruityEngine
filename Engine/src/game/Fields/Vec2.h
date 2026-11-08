#pragma once

#include "game/Field.h"

#include <glm/glm.hpp>

class Vec2Field : public Field {
public:
	void FromJSON(const json &data) override;
	
	glm::vec2 value;
};
