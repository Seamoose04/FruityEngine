#include "MSDFMaterial.h"
#include <iostream>

static const std::string ENGINE_SHADER_PATH = "../../Engine/shaders/materials/";

void MSDFMaterial::FromJSON(const json& j) {
	std::cerr << "[MSDFMaterial] cannot be loaded from JSON." << std::endl;
}

void MSDFMaterial::Init() {
	_shader = std::make_shared<Shader>();
	_shader->Load(ENGINE_SHADER_PATH + "msdf.vert", ENGINE_SHADER_PATH + "msdf.frag");
}

void MSDFMaterial::SetAtlas(Texture* atlas) {
	_atlas = atlas;
}

void MSDFMaterial::SetColor(const glm::vec4& color) {
	_color = color;
}

void MSDFMaterial::Apply() {
	_shader->Use();
	_atlas->Bind(0);
	_shader->SetInt("u_Atlas", 0);
	_shader->SetVec4("u_Color", _color);
}
