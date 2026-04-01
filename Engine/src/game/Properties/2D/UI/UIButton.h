#pragma once

#include "game/ActionRegistry.h"
#include "game/Properties/2D/UI/UIWidget.h"

class UIButton : public UIWidget {
public:
	void FromJSON(const json &j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Draw(Renderer &renderer) override;
	void HandleInput(const Window &window, float dt) override;
	glm::vec2 MeasureContent() override;

private:
	void _Arrange() override;
	bool _Contains(glm::vec2 mousePos) const;
	ActionRegistry::ActionCall _onClick, _onEnter, _onExit, _onRelease;
	bool _hovered = false;
	bool _pressed = false;
	glm::vec2 _lastMousePos{0};
};
