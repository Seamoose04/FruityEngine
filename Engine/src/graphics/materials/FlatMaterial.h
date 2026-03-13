#pragma once

#include "graphics/Material.h"
#include <glm/glm.hpp>

class FlatMaterial : public Material {
	void FromJSON(const json& j) override;
	void Apply() override;

private:
	glm::vec3 _color;
};

