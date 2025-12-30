#pragma once
#include "game/Property.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "game/Scene.h"

class Transform : public Property {
public:
    void FromJSON(const json& data, std::weak_ptr<Scene> scene) override;

	void Rotate(const glm::vec3 &degrees);
	void SetRotation(const glm::vec3 &degrees);

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

	glm::vec3 Forward() const;
	glm::vec3 Right() const;
	glm::vec3 Up() const;

	static inline glm::vec3 forward{0.0f, 0.0f, -1.0f};
	static inline glm::vec3 right{1.0f, 0.0f, 0.0f};
	static inline glm::vec3 up{0.0f, 1.0f, 0.0f};

private:
    glm::quat _cachedQuat{1.0f, 0.0f, 0.0f, 0.0f}; // identity quaternion

    void _UpdateQuaternion();
};
