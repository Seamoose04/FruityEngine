#pragma once

#include "graphics/Material.h"
#include <glm/glm.hpp>

class FlatMaterial : public Material {
public:
	void FromJSON(const json& j) override;
	void Init() override;
	void SetColor(glm::vec4 color);
	void Apply() override;

private:
	glm::vec4 _color;
};

