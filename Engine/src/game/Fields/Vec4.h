#pragma once

#include "game/Field.h"

#include <glm/glm.hpp>

class Vec4Field : public Field {
public:
	void FromJSON(const json &data) override;
	
	glm::vec4 value;
};
