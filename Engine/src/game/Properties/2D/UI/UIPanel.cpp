#include "UIPanel.h"
#include "game/Property.h"
#include "game/PropertyRef.h"

void UIPanel::FromJSON(const json& j) {
	UIContainer::FromJSON(j);
	_flow = j.contains("flow") ? DirectionMap.strToEnum.at(j["flow"]) : Direction::Vertical;
	_gap = j.value("gap", 0.0f);

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
	const Rect& computed = _layout->GetComputedRect();
	Rect inner = {
		computed.x + _padding.left,
		computed.y - _padding.top,
		computed.width - _padding.left - _padding.right,
		computed.height - _padding.top - _padding.bottom
	};

	switch (_flow) {
		case Direction::Vertical: {
			float cursor = inner.y;
			for (auto& child : ActiveChildren()) {
				Rect childAvailable = { inner.x, cursor, inner.width, cursor - (inner.y - inner.height) };
				child->Arrange(childAvailable);
				cursor -= child->GetLayout()->GetComputedRect().height + _gap;
			}
			break;
		}
		case Direction::Horizontal: {
			float cursor = inner.x;
			for (auto& child : ActiveChildren()) {
				Rect childAvailable = { cursor, inner.y, cursor - (inner.x - inner.width), inner.height };
				child->Arrange(childAvailable);
				cursor += child->GetLayout()->GetComputedRect().width + _gap;
			}
			break;
		}
		case Direction::Depth: {
			for (auto& child : ActiveChildren()) {
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
	UIContainer::Draw(renderer);
}

glm::vec2 UIPanel::MeasureContent() {
	if (ActiveChildren().empty()) {
		return glm::vec2(0.0f);
	}
	switch (_flow) {
		case Direction::Vertical: {
			float maxWidth = 0;
			float totalHeight = 0;
			for (auto& child : ActiveChildren()) {
				glm::vec2 childSize = child->MeasureContent();
				
				maxWidth = std::max(maxWidth, childSize.x);
				totalHeight += childSize.y;
			}
			totalHeight += _gap * (ActiveChildren().size() - 1);
			return glm::vec2(maxWidth, totalHeight);
		}
		case Direction::Horizontal: {
			float totalWidth = 0;
			float maxHeight = 0;
			for (auto& child : ActiveChildren()) {
				glm::vec2 childSize = child->MeasureContent();
				
				totalWidth += childSize.x;
				maxHeight = std::max(maxHeight, childSize.y);
			}
			totalWidth += _gap * (ActiveChildren().size() - 1);
			return glm::vec2(totalWidth, maxHeight);
		}
		case Direction::Depth: {
			float maxWidth = 0;
			float maxHeight = 0;
			for (auto& child : ActiveChildren()) {
				glm::vec2 childSize = child->MeasureContent();

				maxWidth = std::max(maxWidth, childSize.x);
				maxHeight = std::max(maxHeight, childSize.y);
			}
			return glm::vec2(maxWidth, maxHeight);
		}
	}
	std::cerr << "[UIPanel::MeasureContent] invalid flow." << std::endl;
	std::abort();
}

REGISTER_PROPERTY(UIPanel)
