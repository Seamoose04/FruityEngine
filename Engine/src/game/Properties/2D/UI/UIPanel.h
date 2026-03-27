#pragma once

#include "graphics/materials/FlatMaterial.h"
#include "game/Properties/2D/UI/UIWidget.h"
#include <glm/glm.hpp>

class UIPanel : public UIWidget {
public:
	void FromJSON(const json &j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void _Arrange() override;
	void Draw(Renderer &renderer) override;

private:
	void _BuildMesh();
	Mesh _mesh;
	glm::vec4 _color;
	Direction _flow;
	float _gap;
	std::shared_ptr<FlatMaterial> _material;
};
