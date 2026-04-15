#pragma once

#include "game/Properties/2D/UI/UIInterfaces.h"
#include "game/Property.h"
#include "game/Scene.h"
#include "game/Properties/2D/UI/UIWidget.h"
#include <glm/fwd.hpp>
#include <vector>
#include <memory>

class UICanvas : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Update(float dt) override;
	void Render(Renderer& renderer) override;
	void OnResize(int width, int height) override;
	void HandleInput(const Window &window, float dt) override;

	void RegisterWidget(std::shared_ptr<UIWidget> widget);
	void UnregisterWidget(UIWidget* widget);
	void Focus(IFocusable* widget);

private:
	bool _justFocused = false;
	IFocusable* _focused;
	std::vector<std::shared_ptr<UIWidget>> _widgets;
	bool _fullscreenX;
	bool _fullscreenY;
	glm::vec2 _screenSize;
	glm::vec2 _size;
	glm::vec2 _anchor;
	glm::vec2 _pivot;
};
