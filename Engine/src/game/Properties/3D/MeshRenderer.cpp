#include "MeshRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "core/FileLoaders/OBJLoader.h"

void MeshRenderer::FromJSON(const json &data) {
	OBJLoader loader;
	std::string meshPath = "assets/meshes/" + (std::string)data["mesh"];
	Mesh* meshPtr = static_cast<Mesh*>(loader.Load(meshPath));
	_mesh = std::move(*meshPtr);
	delete meshPtr;

	_material = Registry<Material>::Instance().Create(data["material"]["type"]);
	_material->FromJSON(data["material"]);
}

void MeshRenderer::OnCreate(std::weak_ptr<Scene> scene) {
	_transform.From(_gameObject);
}

void MeshRenderer::Render(Renderer &renderer) {
	renderer.SubmitMesh(_mesh, _material, _transform->GetWorldMatrix());
}

REGISTER_PROPERTY(MeshRenderer)
