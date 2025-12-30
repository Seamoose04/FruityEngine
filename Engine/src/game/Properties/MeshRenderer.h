#pragma once
#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Properties/Transform.h"
#include "graphics/Mesh.h"
#include "game/Scene.h"

class MeshRenderer : public Property {
public:
	void FromJSON(const json& data, std::weak_ptr<Scene> scene) override;
	void OnCreate() override;
  	void Render(Renderer &renderer) override;

private:
	Mesh _mesh;
	Shader _shader;
	PropertyRef<Transform> _transform;
	std::weak_ptr<Scene> _scene;

	glm::mat4x4 _transformToModelMat();
};
