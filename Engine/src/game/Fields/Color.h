#pragma once

#include <glm/glm.hpp>

#include "game/Field.h"

class ColorField : public Field {
public:
	void FromJSON(const json &data) override;

	glm::vec4 value;
};
