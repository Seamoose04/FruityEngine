#include "MeshRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "core/FileLoaders/OBJLoader.h"
#include "game/Properties/Camera.h"

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
	if (auto lockedScene = _scene.lock()) {
		Camera cam = lockedScene->GetCamera();
		_material->Apply();
		renderer.DrawMesh(_mesh, _material->GetShader(), _transformToModelMat(), cam);
	}
}

glm::mat4x4 MeshRenderer::_transformToModelMat() {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, _transform->position);
  model = glm::rotate(model, glm::radians(_transform->rotation.y),
                      glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(_transform->rotation.x),
                      glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(_transform->rotation.z),
                      glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, _transform->scale);
  return model;
}

REGISTER_PROPERTY(MeshRenderer)
