#pragma once

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
	void RegisterWidget(std::shared_ptr<UIWidget> widget);
	void UnregisterWidget(UIWidget* widget);

private:
	std::vector<std::shared_ptr<UIWidget>> _widgets;
	bool _fullscreenX;
	bool _fullscreenY;
	glm::vec2 _screenSize;
	glm::vec2 _size;
	glm::vec2 _anchor;
	glm::vec2 _pivot;
};
