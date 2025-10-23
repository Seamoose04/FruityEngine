#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);

    void SetPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);
    void SetAspect(float aspectRatio);
    void SetPosition(const glm::vec3& pos);
    void SetYawPitch(float yawDegrees, float pitchDegrees);

    const glm::mat4& GetView() const { return view; }
    const glm::mat4& GetProjection() const { return projection; }

    void Move(const glm::vec3& delta);
    void UpdateView();

    const glm::vec3& GetPosition() const { return position; }
    const glm::vec3& GetForward() const { return forward; }
    const glm::vec3& GetRight() const { return right; }
    const glm::vec3& GetUp() const { return up; }

private:
    glm::mat4 view;
    glm::mat4 projection;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    float yaw;
    float pitch;
};
