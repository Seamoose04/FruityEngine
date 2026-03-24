#pragma once

#include "graphics/Material.h"
#include <glm/glm.hpp>

class FlatMaterial : public Material {
public:
	void FromJSON(const json& j) override;
	void Init();
	void SetColor(glm::vec3 color);
	void Apply() override;

private:
	glm::vec3 _color;
};

