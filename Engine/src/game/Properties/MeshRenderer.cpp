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
	_scene = scene;
	_transform.From(_parent);
}

void MeshRenderer::Render(Renderer &renderer) {
	renderer.SubmitMesh(_mesh, _material, _transformToModelMat());
}

glm::mat4x4 MeshRenderer::_transformToModelMat() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, _transform->GetPosition());
	model *= glm::mat4_cast(_transform->GetRotationQuat());
	model = glm::scale(model, _transform->GetScale());
	return model;
}

REGISTER_PROPERTY(MeshRenderer)
