#include "FlatMaterial.h"

static const std::string ENGINE_MATERIAL_SHADER_PATH = "../../Engine/shaders/materials/";

void FlatMaterial::FromJSON(const json& j) {
	_shader = std::make_shared<Shader>();
	_shader->Load(ENGINE_MATERIAL_SHADER_PATH + "flat.vert", ENGINE_MATERIAL_SHADER_PATH + "flat.frag");

	_color = glm::vec3(j["color"][0], j["color"][1], j["color"][2]);
}

void FlatMaterial::Init() {
	_shader = std::make_shared<Shader>();
	_shader->Load(ENGINE_MATERIAL_SHADER_PATH + "flat.vert", ENGINE_MATERIAL_SHADER_PATH + "flat.frag");
}

void FlatMaterial::SetColor(glm::vec3 color) {
	_color = color;
}

void FlatMaterial::Apply() {
	_shader->Use();
	_shader->SetVec3("u_Color", _color);
}

REGISTER_MATERIAL(FlatMaterial)

