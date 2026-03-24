#include "PointLight.h"
#include "graphics/Renderer.h"

void PointLightProperty::FromJSON(const json& j) {
	light.color = glm::vec3(j["color"][0], j["color"][1], j["color"][2]);
	light.intensity = j["intensity"];
	light.radius = j["radius"];
}

void PointLightProperty::OnCreate(std::weak_ptr<Scene> scene) {
	_transform.From(_gameObject);
}

void PointLightProperty::Render(Renderer& renderer) {
	light.position = _transform->GetPosition();
	renderer.SubmitPointLight(light);
}

REGISTER_PROPERTY(PointLightProperty)
