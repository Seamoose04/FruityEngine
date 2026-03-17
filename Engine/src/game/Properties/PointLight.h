#pragma once

#include "game/Properties/Transform.h"
#include "game/Property.h"
#include "game/PropertyRef.h"
#include "graphics/lights/PointLight.h"

class PointLightProperty : public Property {
public:
	void FromJSON(const json& j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Render(Renderer& renderer) override;

	PointLight light;

private:
	PropertyRef<Transform> _transform;
};
