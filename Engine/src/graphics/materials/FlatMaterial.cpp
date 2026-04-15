#include "FlatMaterial.h"
#include "util/ShaderCache.h"

static const std::string ENGINE_MATERIAL_SHADER_PATH = "../../Engine/shaders/materials/";

void FlatMaterial::FromJSON(const json& j) {
	Init();
	_color = glm::vec4(j["color"][0], j["color"][1], j["color"][2], j.value("opacity", 1.0f));
}

void FlatMaterial::Init() {
	_shader = ShaderCache::Instance().Get(ENGINE_MATERIAL_SHADER_PATH + "flat.vert", ENGINE_MATERIAL_SHADER_PATH + "flat.frag");
}

void FlatMaterial::SetColor(glm::vec4 color) {
	_color = color;
}

void FlatMaterial::Apply() {
	_shader->Use();
	_shader->SetVec4("u_Color", _color);
}

REGISTER_MATERIAL(FlatMaterial)

