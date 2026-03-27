#include "UIPanel.h"
#include "game/Property.h"
#include "game/PropertyRef.h"

void UIPanel::FromJSON(const json& j) {
	UIWidget::FromJSON(j);
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
	_flow = j.contains("flow") ? DirectionMap.strToEnum.at(j["flow"]) : Direction::Vertical;
	_gap = j.value("gap", 0.0f);
}

void UIPanel::OnCreate(std::weak_ptr<Scene> scene) {
	UIWidget::OnCreate(scene);
	_material = std::make_shared<FlatMaterial>();
	_material->Init();
	_material->SetColor(glm::vec3(_color));
	_mesh = Mesh({}, {}, Mesh::DrawHint::Dynamic);
	_BuildMesh();
}

void UIPanel::_Arrange() {
	const Rect& computed = _layout->GetComputedRect();
	const Sides& padding = _layout->GetPadding();
	Rect inner = {
		computed.x + padding.left,
		computed.y - padding.top,
		computed.width - padding.left - padding.right,
		computed.height - padding.top - padding.bottom
	};

	switch (_flow) {
		case Direction::Vertical: {
			float cursor = inner.y;
			for (auto& child : _children) {
				Rect childAvailable = { inner.x, cursor, inner.width, inner.height - (inner.y - cursor) };
				child->Arrange(childAvailable);
				cursor -= child->GetLayout()->GetComputedRect().height + _gap;
			}
			break;
		}
		case Direction::Horizontal: {
			float cursor = inner.x;
			for (auto& child : _children) {
				Rect childAvailable = { cursor, inner.y, inner.width - (cursor - inner.x), inner.height };
				child->Arrange(childAvailable);
				cursor += child->GetLayout()->GetComputedRect().width + _gap;
			}
			break;
		}
		case Direction::Depth: {
			for (auto& child : _children) {
				child->Arrange(inner);
			}
			break;
		}
	}
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
	for (auto& child : _children) {
        child->Draw(renderer);
    }
}

REGISTER_PROPERTY(UIPanel)
