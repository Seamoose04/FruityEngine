#include "Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

void Transform::FromJSON(const json &data, std::weak_ptr<Scene> scene) {
	if (data.contains("position")) {
		position = glm::vec3(data["position"][0], data["position"][1],
					   data["position"][2]);
	}

	if (data.contains("rotation")) {
		rotation = glm::vec3(data["rotation"][0], data["rotation"][1],
					   data["rotation"][2]);
	}

	if (data.contains("scale")) {
		scale = glm::vec3(data["scale"][0], data["scale"][1], data["scale"][2]);
	}
}

void Transform::Rotate(const glm::vec3 &degrees) {
  rotation += degrees;
  _UpdateQuaternion();
}

void Transform::SetRotation(const glm::vec3 &degrees) {
	rotation = degrees;
	_UpdateQuaternion();
}

glm::vec3 Transform::Forward() const {
	return glm::normalize(_cachedQuat * glm::vec3(0, 0, -1));
}

glm::vec3 Transform::Right() const {
	return glm::normalize(_cachedQuat * glm::vec3(1, 0, 0));
}

glm::vec3 Transform::Up() const {
	return glm::normalize(_cachedQuat * glm::vec3(0, 1, 0));
}

void Transform::_UpdateQuaternion() {
	_cachedQuat = glm::quat(glm::radians(rotation));
}

REGISTER_PROPERTY(Transform)
