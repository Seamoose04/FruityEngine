#include "UIButton.h"
#include "GLFW/glfw3.h"
#include "game/ActionRegistry.h"

void UIButton::FromJSON(const json& j) {
	UIWidget::FromJSON(j);
	_onClick = j.value("onClick", "");
	_onRelease = j.value("onRelease", "");
	_onEnter = j.value("onEnter", "");
	_onExit = j.value("onExit", "");
}

void UIButton::OnCreate(std::weak_ptr<Scene> scene) {
	UIWidget::OnCreate(scene);
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
		ActionRegistry::Instance().Execute(_onEnter);
	}
	if (!_onExit.empty() && !_hovered && wasHovered) {
		ActionRegistry::Instance().Execute(_onExit);
	}
	if (!_onClick.empty() && _pressed && !wasPressed) {
		ActionRegistry::Instance().Execute(_onClick);
	}
	if (!_onRelease.empty() && !_pressed && wasPressed) {
		ActionRegistry::Instance().Execute(_onRelease);
	}
}

bool UIButton::_Contains(glm::vec2 mousePos) const {
	const Rect& computedRect = _layout->GetComputedRect();
	Rect adjusted = { computedRect.x, computedRect.y - computedRect.height, computedRect.width, computedRect.height };
	return adjusted.Contains(mousePos);
}

REGISTER_PROPERTY(UIButton)
