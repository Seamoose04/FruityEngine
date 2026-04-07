#pragma once

#include "game/ActionRegistry.h"
#include "game/Properties/2D/UI/UIContainer.h"

class UIButton : public UIContainer {
public:
	void FromJSON(const json &j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void HandleInput(const Window &window, float dt) override;
	void SetOnClick(std::function<void()> callback);
	void SetOnEnter(std::function<void()> callback);
	void SetOnExit(std::function<void()> callback);
	void SetOnRelease(std::function<void()> callback);
	Callback onClick, onEnter, onExit, onRelease;

private:
	bool _Contains(glm::vec2 mousePos) const;
	bool _hovered = false;
	bool _pressed = false;
	glm::vec2 _lastMousePos{0};
};
