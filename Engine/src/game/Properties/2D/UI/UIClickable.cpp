#include "UIClickable.h"
#include "GLFW/glfw3.h"

void UIClickable::FromJSON(const json& j) {
	UIContainer::FromJSON(j);
}

void UIClickable::HandleInput(const Window& window, float dt) {
	bool wasHovered = _hovered;
	bool wasPressed = _pressed;

	glm::vec2 worldPos = _camera->ScreenToWorld(window.GetMousePos());
	_hovered =  _Contains(worldPos);
	_pressed = _hovered && window.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

	if (_hovered && !wasHovered) {
		onEnter.Call();
	}
	if (!_hovered && wasHovered) {
		onExit.Call();
	}
	if (_pressed && !wasPressed) {
		onClick.Call();
	}
	if (!_pressed && wasPressed) {
		onRelease.Call();
	}
}

bool UIClickable::_Contains(glm::vec2 mousePos) const {
	const Rect& computedRect = _layout->GetComputedRect();
	Rect adjusted = { computedRect.x, computedRect.y - computedRect.height, computedRect.width, computedRect.height };
	return adjusted.Contains(mousePos);
}

glm::vec2 UIClickable::MeasureContent() {
	if (ActiveChildren().empty()) {
		return glm::vec2(0.0f);
	}
	float maxWidth = 0;
	float maxHeight = 0;
	for (auto& child : ActiveChildren()) {
		glm::vec2 childSize = child->MeasureContent();

		maxWidth = std::max(maxWidth, childSize.x);
		maxHeight = std::max(maxHeight, childSize.y);
	}
	return glm::vec2(maxWidth, maxHeight);
}

void UIClickable::_Arrange() {
	UIContainer::_Arrange();
	const Rect& computed = _layout->GetComputedRect();
	Rect inner = {
		computed.x + _padding.left,
		computed.y - _padding.top,
		computed.width - _padding.left - _padding.right,
		computed.height - _padding.top - _padding.bottom
	};
	for (auto& child : ActiveChildren()) {
		child->Arrange(inner);
	}
}
