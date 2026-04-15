#pragma once

#include "game/ActionRegistry.h"
#include "game/Properties/2D/UI/UIClickable.h"

class UIButton : public UIClickable {
public:
	void FromJSON(const json &j) override;
};
