#include "Camera.h"

#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "game/Scene.h"

void Camera::FromJSON(const json &data) {
	_near = data["near"];
	_far = data["far"];

	if (data["mode"] == "perspective") {
		_mode = ProjectionMode::Perspective;
		_fov = data["fov"];
	} else if (data["mode"] == "orthographic") {
		_mode = ProjectionMode::Orthographic;
	}
}

void Camera::OnCreate(std::weak_ptr<Scene> scene) {
	_scene = scene;
	_transform.From(_parent);

	if (auto lockedScene = _scene.lock()) {
		auto self = shared_from_this();
		lockedScene->SetCamera(std::dynamic_pointer_cast<Camera>(self));
	}

	UpdateProjection();
	UpdateView();
}

void Camera::Render(Renderer& renderer) {
	renderer.SetCamera(this, _transform->GetPosition());
}

void Camera::UpdateProjection() {
	if (_mode == ProjectionMode::Perspective) {
		float vFov = 2.0f * glm::atan(glm::tan(glm::radians(_fov) / 2.0f) / _aspectRatio);
		_projection = glm::perspective(vFov, _aspectRatio, _near, _far);
	} else if (_mode == ProjectionMode::Orthographic) {
		_projection = glm::ortho(-(float)_screenWidth / 2, (float)_screenWidth / 2, -(float)_screenHeight / 2, (float)_screenHeight / 2, _near, _far);
	}
}

void Camera::UpdateView() {
	if (_mode == ProjectionMode::Perspective) {
		_view = glm::lookAt(_transform->GetPosition(), _transform->GetPosition() + _transform->Forward(), Transform::up);
	} else if (_mode == ProjectionMode::Orthographic) {
		_view = glm::identity<glm::mat4>();
	}
}

void Camera::UpdateAspectRatio(int width, int height) {
	_screenWidth = width;
	_screenHeight = height;
	_aspectRatio = (float)width / height;
	UpdateProjection();
}

glm::mat4 Camera::GetView() const {
	return _view;
}

glm::mat4 Camera::GetProjection() const { return _projection; }

REGISTER_PROPERTY(Camera)
