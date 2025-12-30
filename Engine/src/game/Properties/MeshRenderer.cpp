#include "MeshRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "core/FileLoaders/OBJLoader.h"
#include "game/Properties/Camera.h"
#include "graphics/Shader.h"

void MeshRenderer::FromJSON(const json &data, std::weak_ptr<Scene> scene) {
	_scene = scene;

	if (data.contains("mesh")) {
		OBJLoader loader;
		std::string meshPath = "Game/assets/meshes/" + (std::string)data["mesh"];
		Mesh* meshPtr = static_cast<Mesh*>(loader.Load(meshPath));
        _mesh = std::move(*meshPtr);
		delete meshPtr;
	} else {
		_mesh = Mesh();
	}

	if (data.contains("shader")) {
		json shaderData = data["shader"];
		if (shaderData.contains("vertex") && shaderData.contains("fragment")) {
			std::string vPath = "Game/assets/shaders/" + (std::string)shaderData["vertex"];
			std::string fPath = "Game/assets/shaders/" + (std::string)shaderData["fragment"];
			_shader.Load(vPath, fPath);
		}
	}
}

void MeshRenderer::OnCreate() {
	_transform.From(_parent);
}

void MeshRenderer::Render(Renderer &renderer) {
	if (auto lockedScene = _scene.lock()) {
		Camera cam = lockedScene->GetCamera();
		renderer.DrawMesh(_mesh, _shader, _transformToModelMat(), cam);
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
