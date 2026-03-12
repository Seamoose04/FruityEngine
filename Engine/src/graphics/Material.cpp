#include "Material.h"

std::shared_ptr<Shader> Material::GetShader() {
	return _shader;
}
