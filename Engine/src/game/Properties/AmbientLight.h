#pragma once

#include "game/Property.h"
#include "graphics/lights/AmbientLight.h"

class AmbientLightProperty : public Property {
public:
	void FromJSON(const json& j) override;
	void Render(Renderer& renderer) override;
	AmbientLight light;
};
