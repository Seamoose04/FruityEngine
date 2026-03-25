#pragma once

#include "game/Property.h"
#include "game/Scene.h"
#include "game/Properties/2D/UI/UIWidget.h"
#include <glm/fwd.hpp>
#include <vector>

class UICanvas : public Property {
public:
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Update(float dt) override;
	void Render(Renderer& renderer) override;
	void OnResize(int width, int height) override;
	void RegisterWidget(UIWidget* widget);
	void MarkDirty();

private:
	std::vector<UIWidget*> _widgets;
	bool _dirty = true;
	glm::vec2 _screenSize;
};
