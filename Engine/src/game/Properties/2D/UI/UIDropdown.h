#pragma once

#include "game/Properties/2D/UI/UIContainer.h"
#include "game/Properties/2D/UI/UIButton.h"

class UIDropdown : public UIContainer {
public:
	void FromJSON(const json &j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	glm::vec2 MeasureContent() override;

private:
	void _Arrange() override;

	std::shared_ptr<UIButton> _bar;
	std::shared_ptr<UIContainer> _body;

	json _barChunk;
	json _bodyChunk;
	bool _open;
};
