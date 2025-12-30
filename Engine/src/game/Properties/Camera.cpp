#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <glm/gtx/string_cast.hpp>
#include <memory>

void Camera::FromJSON(const json &data, std::weak_ptr<Scene> scene) {
	_fov = data["fov"];
	_near = data["near"];
	_far = data["far"];

	_scene = scene;
}

void Camera::OnCreate() {
	_transform.From(_parent);

	if (auto lockedScene = _scene.lock()) {
		auto self = shared_from_this();
		lockedScene->SetCamera(std::dynamic_pointer_cast<Camera>(self));
	}

	UpdatePerspective();
	UpdateView();
}

void Camera::UpdatePerspective() {
	_projection = glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
}

void Camera::UpdateView() {
	_view = glm::lookAt(_transform->position, _transform->position + _transform->Forward(), Transform::up);
}

void Camera::UpdateAspectRatio(int width, int height) {
	_aspectRatio = (float)width / height;
	UpdatePerspective();
}

glm::mat4 Camera::GetView() const {
	return _view;
}

glm::mat4 Camera::GetProjection() const { return _projection; }

REGISTER_PROPERTY(Camera)
