#pragma once

#include <json.hpp>
#include <memory.h>

#include "graphics/Shader.h"
#include "util/Registry.h"

using json = nlohmann::json;

class Material {
public:
	virtual void FromJSON(const json& j) = 0;
	virtual void Apply() = 0;
	virtual ~Material() = default;
	std::shared_ptr<Shader> GetShader();

protected:
	std::shared_ptr<Shader> _shader;
};

#define REGISTER_MATERIAL(DerivedType) REGISTER_TYPE(Material, DerivedType)
