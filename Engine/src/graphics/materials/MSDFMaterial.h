#pragma once

#include "graphics/Texture.h"
#include "graphics/Material.h"

class MSDFMaterial : public Material {
public:
	void SetAtlas(Texture* atlas);
	void SetColor(const glm::vec4& color);
	void FromJSON(const json& j) override;
	void Apply() override;
	void Init();
	
private:
	Texture* _atlas = nullptr;
	glm::vec4 _color = glm::vec4(1.0f);
};
