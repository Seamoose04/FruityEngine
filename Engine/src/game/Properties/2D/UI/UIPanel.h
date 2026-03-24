#pragma once

#include "graphics/materials/FlatMaterial.h"
#include "game/Properties/2D/UI/UIWidget.h"
#include "game/Properties/2D/UITypes.h"
#include <glm/glm.hpp>

class UIPanel : public UIWidget {
public:
	void FromJSON(const json &j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Arrange(Rect availableRect) override;
	void Draw(Renderer &renderer) override;
	float ResolveAxis(Size size, float available);

private:
	void _BuildMesh();
	Mesh _mesh;
	glm::vec4 _color;
	Direction _flow;
	float _gap;
	std::shared_ptr<FlatMaterial> _material;
};
