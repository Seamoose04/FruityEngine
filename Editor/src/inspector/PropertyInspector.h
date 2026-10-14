#pragma once

#include "game/Properties/2D/UI/UIWidget.h"
#include "game/Property.h"

class PropertyInspector : public UIWidget {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Draw(Renderer &renderer) override;

private:
	void _Arrange() override;
	json _property;
};

