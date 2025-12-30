#include "MeshRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "core/FileLoaders/OBJLoader.h"
#include "graphics/Shader.h"

void MeshRenderer::FromJSON(const json &data, const std::string& scenePath) {
	if (data.contains("mesh")) {
		OBJLoader loader;
		std::string meshPath = scenePath + "/" + (std::string)data["mesh"];
		_mesh = *static_cast<Mesh*>(loader.Load(meshPath));
	} else {
		_mesh = Mesh();
	}
	if (data.contains("shader")) {
		json shaderData = data["shader"];
		if (shaderData.contains("vertex") && shaderData.contains("fragment")) {
			std::string vPath = scenePath + "/" + (std::string)shaderData["vertex"];
			std::string fPath = scenePath + "/" + (std::string)shaderData["fragment"];
			_shader.Load(vPath, fPath);
		}
	}
}

void MeshRenderer::OnCreate() {
	_transform.SetParent(_parent);
	_transform.Resolve();
}

void MeshRenderer::Render(Renderer &renderer) {
	GLint va, vb, ebo, enabled = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &va);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vb);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
	glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
	std::cout << "VAO=" << va << " VBO=" << vb
		<< " EBO=" << ebo
		<< " attrib0=" << enabled
		<< " indices=" << _mesh.GetIndexCount() << std::endl;

	renderer.DrawMesh(_mesh, _shader, _transformToModelMat());
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
