#include "Transform.h"
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "game/GameObject.h"

void Transform::FromJSON(const json &data) {
	if (data.contains("position")) {
		_localPosition = glm::vec3(data["position"][0], data["position"][1],
					   data["position"][2]);
	}

	if (data.contains("rotation")) {
		_localRotation = glm::vec3(data["rotation"][0], data["rotation"][1],
					   data["rotation"][2]);
	}

	if (data.contains("scale")) {
		_localScale = glm::vec3(data["scale"][0], data["scale"][1], data["scale"][2]);
	}

	_cacheDirty = true;
}

void Transform::Update(float dt) {
	_updatedThisFrame = false;
	GameObject* parent = nullptr;
	if (auto lockedGameObject = GetParent().lock()) {
		if (auto lockedParent = lockedGameObject->GetParent().lock()) {
			parent = lockedParent.get();
		}
	}
	if (_cacheDirty || (parent != nullptr && parent->GetProperty<Transform>()->_updatedThisFrame)) {
		_UpdateQuaternion();
		_UpdateLocalMatrix();
		_UpdateWorldMatrix(parent != nullptr ? parent->GetProperty<Transform>()->GetWorldMatrix() : glm::mat4(1.0f));
		_updatedThisFrame = true;
		_cacheDirty = false;
	}
}

void Transform::SetLocalPosition(const glm::vec3& pos) {
	_cacheDirty = true;
	_localPosition = pos;
}

void Transform::SetLocalRotation(const glm::vec3& rot) {
	_cacheDirty = true;
	_localRotation = rot;
}

void Transform::SetLocalScale(const glm::vec3& scale) {
	_cacheDirty = true;
	_localScale = scale;
}

void Transform::Translate(const glm::vec3& delta) {
	_cacheDirty = true;
	_localPosition += delta;
}

void Transform::Rotate(const glm::vec3& degrees) {
	_cacheDirty = true;
	_localRotation += degrees;
}

void Transform::ScaleBy(const glm::vec3& delta) {
	_cacheDirty = true;
	_localScale *= delta;
}

const glm::vec3& Transform::GetLocalPosition() const {
	return _localPosition;
}

const glm::vec3& Transform::GetLocalRotation() const {
	return _localRotation;
}

const glm::quat& Transform::GetLocalRotationQuat() const {
	return _cachedQuat;
}

const glm::vec3& Transform::GetLocalScale() const {
	return _localScale;
}

glm::vec3 Transform::GetPosition() const {
	return glm::vec3(_cachedWorldMatrix[3]);
}

glm::vec3 Transform::GetRotation() const {
	return glm::degrees(glm::eulerAngles(GetRotationQuat()));	
}

glm::quat Transform::GetRotationQuat() const {
	glm::vec3 scale = GetScale();
	glm::mat4 rotMatrix = _cachedWorldMatrix;
	rotMatrix[0] /= scale.x;
	rotMatrix[1] /= scale.y;
	rotMatrix[2] /= scale.z;
	return glm::quat_cast(rotMatrix);
}

glm::vec3 Transform::GetScale() const {
	return glm::vec3(
		glm::length(glm::vec3(_cachedWorldMatrix[0])),
		glm::length(glm::vec3(_cachedWorldMatrix[1])),
		glm::length(glm::vec3(_cachedWorldMatrix[2]))
	);
}

const glm::mat4& Transform::GetLocalMatrix() const {
	return _cachedLocalMatrix;
}

const glm::mat4& Transform::GetWorldMatrix() const {
	return _cachedWorldMatrix;
}

glm::vec3 Transform::Forward() const {
	return glm::normalize(GetRotationQuat() * glm::vec3(0, 0, -1));
}

glm::vec3 Transform::Right() const {
	return glm::normalize(GetRotationQuat() * glm::vec3(1, 0, 0));
}

glm::vec3 Transform::Up() const {
	return glm::normalize(GetRotationQuat() * glm::vec3(0, 1, 0));
}

glm::vec3 Transform::LocalForward() const {
	return glm::normalize(GetLocalRotationQuat() * glm::vec3(0, 0, -1));
}

glm::vec3 Transform::LocalRight() const {
	return glm::normalize(GetLocalRotationQuat() * glm::vec3(1, 0, 0));
}

glm::vec3 Transform::LocalUp() const {
	return glm::normalize(GetLocalRotationQuat() * glm::vec3(0, 1, 0));
}

void Transform::_UpdateQuaternion() {
	_cachedQuat = glm::quat(glm::radians(GetLocalRotation()));
}

void Transform::_UpdateLocalMatrix() {
	glm::mat4 T = glm::translate(glm::mat4(1.0f), _localPosition);
	glm::mat4 R = glm::toMat4(_cachedQuat);
	glm::mat4 S = glm::scale(glm::mat4(1.0f), _localScale);
	_cachedLocalMatrix = T * R * S;
}

void Transform::_UpdateWorldMatrix(const glm::mat4& parentWorldMatrix) {
	_cachedWorldMatrix = parentWorldMatrix * _cachedLocalMatrix;
}

REGISTER_PROPERTY(Transform)
