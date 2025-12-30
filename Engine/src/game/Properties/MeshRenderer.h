#pragma once
#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Properties/Transform.h"
#include "graphics/Mesh.h"

class MeshRenderer : public Property {
public:
	void FromJSON(const json& data, const std::string &scenePath) override;
	void OnCreate() override;
  	void Render(Renderer &renderer) override;

private:
	Mesh _mesh;
	Shader _shader;
	PropertyRef<Transform> _transform;

	glm::mat4x4 _transformToModelMat();
};
