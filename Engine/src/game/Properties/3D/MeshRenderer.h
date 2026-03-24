#pragma once
#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Properties/3D/Transform.h"
#include "graphics/Mesh.h"
#include "game/Scene.h"
#include "graphics/Material.h"

class MeshRenderer : public Property {
public:
	void FromJSON(const json& data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
  	void Render(Renderer &renderer) override;

private:
	Mesh _mesh;
	std::shared_ptr<Material> _material;
	PropertyRef<Transform> _transform;
};
