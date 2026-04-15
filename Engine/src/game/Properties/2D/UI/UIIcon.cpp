#include "UIIcon.h"

void UIIcon::FromJSON(const json& j) {
	UIWidget::FromJSON(j);
	if (j.contains("color")) {
		_color = glm::vec4(
			j["color"][0],
			j["color"][1],
			j["color"][2],
			j["color"][3]
		);
	} else {
		_color = glm::vec4(1.0f);
	}
}

void UIIcon::OnCreate(std::weak_ptr<Scene> scene) {
	UIWidget::OnCreate(scene);
	_material = std::make_shared<FlatMaterial>();
	_material->Init();
	_UpdateMaterial();
	_mesh = Mesh({}, {}, Mesh::DrawHint::Dynamic);
	_BuildMesh();
}

void UIIcon::_UpdateMaterial() {
	_material->SetColor(_color);
}

void UIIcon::Draw(Renderer& renderer) {
	if (_meshDirty) {
		_BuildMesh();
	}
	renderer.SubmitMesh(_mesh, _material, glm::mat4(1.0f),
		[]() { glDisable(GL_DEPTH_TEST); },
		[]() { glEnable(GL_DEPTH_TEST); }
	);
}

void UIIcon::SetColor(const glm::vec4& color) {
	_color = color;
	_UpdateMaterial();
}

void UIIcon::_Arrange() {
	_meshDirty = true;
}

void UIIcon::_BuildMesh() {
	const Rect& computedRect = _layout->GetComputedRect();
	float x = computedRect.x;
	float y = computedRect.y;
	float w = computedRect.width;
	float h = computedRect.height;

	std::vector<Vertex> vertices = {
		{{ x,     y - h,     0 }, { 0, 0, 1 }, { 0, 0 }},
		{{ x + w, y - h,     0 }, { 0, 0, 1 }, { 1, 0 }},
		{{ x,     y, 0 }, { 0, 0, 1 }, { 0, 1 }},
		{{ x + w, y, 0 }, { 0, 0, 1 }, { 1, 1 }},
	};
	std::vector<unsigned int> indices = { 0, 1, 2, 1, 3, 2 };
	_mesh.Upload(vertices, indices);
}

REGISTER_PROPERTY(UIIcon)
