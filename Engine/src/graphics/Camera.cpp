#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

Camera::Camera()
    : position(0.0f, 0.0f, 3.0f),
      yaw(-90.0f),
      pitch(0.0f)
{
    SetPerspective(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    UpdateView();
}

Camera::Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
    : position(0.0f, 0.0f, 3.0f),
      yaw(-90.0f),
      pitch(0.0f)
{
    SetPerspective(fovDegrees, aspectRatio, nearPlane, farPlane);
    UpdateView();
}

void Camera::SetPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane) {
    projection = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearPlane, farPlane);
}

void Camera::SetAspect(float aspectRatio) {
    float fov = glm::degrees(2.0f * atan(1.0f / projection[1][1]));
    SetPerspective(fov, aspectRatio, 0.1f, 100.0f);
}

void Camera::SetPosition(const glm::vec3& pos) {
    position = pos;
    UpdateView();
}

void Camera::SetYawPitch(float yawDegrees, float pitchDegrees) {
    yaw = yawDegrees;
    pitch = glm::clamp(pitchDegrees, -89.0f, 89.0f);
    UpdateView();
}

void Camera::Move(const glm::vec3& delta) {
    position += delta;
    UpdateView();
}

void Camera::UpdateView() {
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(dir);

    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));

    view = glm::lookAt(position, position + forward, up);
}
