#include "LitMaterial.h"
#include "graphics/Material.h"
#include "util/ShaderCache.h"

static const std::string ENGINE_MATERIAL_SHADER_PATH = "../../Engine/shaders/materials/";

void LitMaterial::FromJSON(const json& j) {
	Init();

	_color = glm::vec3(j["color"][0], j["color"][1], j["color"][2]);
	_roughness = j["roughness"];
	_metalness = j["metalness"];
}

void LitMaterial::Init() {
	_shader = ShaderCache::Instance().Get(ENGINE_MATERIAL_SHADER_PATH + "lit.vert", ENGINE_MATERIAL_SHADER_PATH + "lit.frag");
}

void LitMaterial::Apply() {
	_shader->Use();
	_shader->SetVec3("u_Color", _color);
	_shader->SetFloat("u_Roughness", _roughness);
	_shader->SetFloat("u_Metalness", _metalness);
}

REGISTER_MATERIAL(LitMaterial)
