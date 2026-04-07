#include "UIPanel.h"
#include "game/Property.h"
#include "game/PropertyRef.h"

void UIPanel::FromJSON(const json& j) {
	UIContainer::FromJSON(j);
	if (j.contains("color")) {
		_color = glm::vec4({
			j["color"][0],
			j["color"][1],
			j["color"][2],
			j["color"][3]
		});
	} else {
		_color = glm::vec4(0.0f);
	}
}

void UIPanel::OnCreate(std::weak_ptr<Scene> scene) {
	UIWidget::OnCreate(scene);
	_material = std::make_shared<FlatMaterial>();
	_material->Init();
	_UpdateMaterial();
	_mesh = Mesh({}, {}, Mesh::DrawHint::Dynamic);
	_BuildMesh();
}

void UIPanel::SetColor(const glm::vec4& color) {
	_color = color;
	_UpdateMaterial();
}

void UIPanel::_UpdateMaterial() {
	_material->SetColor(_color);
}

void UIPanel::_Arrange() {
	UIContainer::_Arrange();
	_BuildMesh();
}

void UIPanel::_BuildMesh() {
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

void UIPanel::Draw(Renderer& renderer) {
	renderer.SubmitMesh(_mesh, _material, glm::mat4(1.0f),
		[]() { glDisable(GL_DEPTH_TEST); },
		[]() { glEnable(GL_DEPTH_TEST); }
	);
	UIContainer::Draw(renderer);
}

REGISTER_PROPERTY(UIPanel)
