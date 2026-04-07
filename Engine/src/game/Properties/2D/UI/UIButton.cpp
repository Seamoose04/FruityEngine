#include "UIButton.h"
#include "GLFW/glfw3.h"

void UIButton::FromJSON(const json& j) {
	UIContainer::FromJSON(j);
	if (j.contains("onClick")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onClick"]);
		onClick.Subscribe([call]() {
			call.Invoke();
		});
	}
	if (j.contains("onRelease")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onRelease"]);
		onRelease.Subscribe([call]() {
			call.Invoke();
		});
	}
	if (j.contains("onEnter")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onEnter"]);
		onEnter.Subscribe([call]() {
			call.Invoke();
		});
	}
	if (j.contains("onExit")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onExit"]);
		onExit.Subscribe([call]() {
			call.Invoke();
		});
	}
}

void UIButton::OnCreate(std::weak_ptr<Scene> scene) {
	UIContainer::OnCreate(scene);
}

void UIButton::HandleInput(const Window& window, float dt) {
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

bool UIButton::_Contains(glm::vec2 mousePos) const {
	const Rect& computedRect = _layout->GetComputedRect();
	Rect adjusted = { computedRect.x, computedRect.y - computedRect.height, computedRect.width, computedRect.height };
	return adjusted.Contains(mousePos);
}

REGISTER_PROPERTY(UIButton)
