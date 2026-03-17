#include "AmbientLight.h"

#include "game/Property.h"
#include "graphics/Renderer.h"

void AmbientLightProperty::FromJSON(const json& j) {
	light.color = glm::vec3(j["color"][0], j["color"][1], j["color"][2]);
	light.intensity = j["intensity"];
}

void AmbientLightProperty::Render(Renderer& renderer) {
	renderer.SetAmbientLight(light);
}

REGISTER_PROPERTY(AmbientLightProperty)
