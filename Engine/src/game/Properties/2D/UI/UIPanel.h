#pragma once

#include "graphics/materials/FlatMaterial.h"
#include "game/Properties/2D/UI/UIContainer.h"
#include <glm/glm.hpp>

class UIPanel : public UIContainer {
public:
	void FromJSON(const json &j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Draw(Renderer &renderer) override;
	void SetColor(const glm::vec4& color);

private:
	void _Arrange() override;
	void _BuildMesh();
	void _UpdateMaterial();
	Mesh _mesh;
	glm::vec4 _color;
	std::shared_ptr<FlatMaterial> _material;
};
