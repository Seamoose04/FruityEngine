#include "EmissiveMaterial.h"

static const std::string ENGINE_MATERIAL_SHADER_PATH = "../../Engine/shaders/materials/";

void EmissiveMaterial::FromJSON(const json& j) {
	_shader = std::make_shared<Shader>();
	_shader->Load(ENGINE_MATERIAL_SHADER_PATH + "emissive.vert", ENGINE_MATERIAL_SHADER_PATH + "emissive.frag");

	_color = glm::vec3(j["color"][0], j["color"][1], j["color"][2]);
	_intensity = j["intensity"];
}

void EmissiveMaterial::Apply() {
	_shader->Use();
	_shader->SetVec3("u_Color", _color);
	_shader->SetFloat("u_Intensity", _intensity);
}

REGISTER_MATERIAL(EmissiveMaterial)
