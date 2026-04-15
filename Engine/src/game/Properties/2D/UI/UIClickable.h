#pragma once

#include "game/Properties/2D/UI/UIContainer.h"

class UIClickable : public UIContainer {
public:
	void FromJSON(const json &j) override;
	void HandleInput(const Window &window, float dt) override;
	glm::vec2 MeasureContent() override;
	Callback<> onClick, onEnter, onExit, onRelease;

private:
	void _Arrange() override;
	bool _Contains(glm::vec2 mousePos) const;
	bool _hovered = false;
	bool _pressed = false;
	glm::vec2 _lastMousePos{0};
};
