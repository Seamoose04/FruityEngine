#pragma once

#include "graphics/Material.h"

#include <glm/glm.hpp>

class LitMaterial : public Material {
public:
	void FromJSON(const json& j) override;
	void Apply() override;

private:
	glm::vec3 _color;
	float _roughness;
	float _metalness;
};
