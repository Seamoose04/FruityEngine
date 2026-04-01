#include "UIButton.h"
#include "GLFW/glfw3.h"

void UIButton::FromJSON(const json& j) {
	UIWidget::FromJSON(j);
	if (j.contains("onClick")) {
		_onClick.FromJSON(j["onClick"]);
	}
	if (j.contains("onRelease")) {
		_onRelease.FromJSON(j["onRelease"]);
	}
	if (j.contains("onEnter")) {
		_onEnter.FromJSON(j["onEnter"]);
	}
	if (j.contains("onExit")) {
		_onExit.FromJSON(j["onExit"]);
	}
}

void UIButton::OnCreate(std::weak_ptr<Scene> scene) {
	UIWidget::OnCreate(scene);
}

glm::vec2 UIButton::MeasureContent() {
	if (_children.empty()) {
		return glm::vec2(0.0f);
	}
	float maxWidth = 0;
	float maxHeight = 0;
	for (auto& child : _children) {
		glm::vec2 childSize = child->MeasureContent();

		maxWidth = std::max(maxWidth, childSize.x);
		maxHeight = std::max(maxHeight, childSize.y);
	}
	return glm::vec2(maxWidth, maxHeight);
}

void UIButton::Draw(Renderer& renderer) {
	for (auto& child : _children) {
        child->Draw(renderer);
    }
}

void UIButton::_Arrange() {
	const Rect& computed = _layout->GetComputedRect();
	const Sides& padding = _layout->GetPadding();
	Rect inner = {
		computed.x + padding.left,
		computed.y - padding.top,
		computed.width - padding.left - padding.right,
		computed.height - padding.top - padding.bottom
	};
	for (auto& child : _children) {
		child->Arrange(inner);
	}
}

void UIButton::HandleInput(const Window& window, float dt) {
	bool wasHovered = _hovered;
	bool wasPressed = _pressed;

	glm::vec2 worldPos = _camera->ScreenToWorld(window.GetMousePos());
	_hovered =  _Contains(worldPos);
	_pressed = _hovered && window.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

	if (!_onEnter.empty() && _hovered && !wasHovered) {
		_onEnter.Invoke();
	}
	if (!_onExit.empty() && !_hovered && wasHovered) {
		_onExit.Invoke();
	}
	if (!_onClick.empty() && _pressed && !wasPressed) {
		_onClick.Invoke();
	}
	if (!_onRelease.empty() && !_pressed && wasPressed) {
		_onRelease.Invoke();
	}
}

bool UIButton::_Contains(glm::vec2 mousePos) const {
	const Rect& computedRect = _layout->GetComputedRect();
	Rect adjusted = { computedRect.x, computedRect.y - computedRect.height, computedRect.width, computedRect.height };
	return adjusted.Contains(mousePos);
}

REGISTER_PROPERTY(UIButton)
