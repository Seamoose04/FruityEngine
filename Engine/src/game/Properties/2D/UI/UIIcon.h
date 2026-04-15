#pragma once

#include "UIWidget.h"
#include "graphics/materials/FlatMaterial.h"
#include "graphics/Mesh.h"
#include <memory>

class UIIcon : public UIWidget {
public:
	void FromJSON(const json &j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Draw(Renderer &renderer) override;

	void SetColor(const glm::vec4& color);
	
private:
	void _Arrange() override;
	void _BuildMesh();
	void _UpdateMaterial();

	glm::vec4 _color;
	Mesh _mesh;
	std::shared_ptr<FlatMaterial> _material;
	bool _meshDirty = true;
};
