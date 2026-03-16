#pragma once
#include "game/Property.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Transform : public Property {
public:
    void FromJSON(const json& data) override;
	void Update(float ft) override;

	void SetLocalPosition(const glm::vec3& pos);
	void SetLocalRotation(const glm::vec3& rot);
	void SetLocalScale(const glm::vec3& scale);

	void Translate(const glm::vec3& delta);
	void Rotate(const glm::vec3& degrees);
	void ScaleBy(const glm::vec3& delta);
	
	const glm::vec3& GetLocalPosition() const;
	const glm::vec3& GetLocalRotation() const;
	const glm::quat& GetLocalRotationQuat() const;
	const glm::vec3& GetLocalScale() const;

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::quat GetRotationQuat() const;
	glm::vec3 GetScale() const;

	const glm::mat4& GetLocalMatrix() const;
	const glm::mat4& GetWorldMatrix() const;

	glm::vec3 Forward() const;
	glm::vec3 Right() const;
	glm::vec3 Up() const;

	glm::vec3 LocalForward() const;
	glm::vec3 LocalRight() const;
	glm::vec3 LocalUp() const;

	static inline glm::vec3 forward{0.0f, 0.0f, -1.0f};
	static inline glm::vec3 right{1.0f, 0.0f, 0.0f};
	static inline glm::vec3 up{0.0f, 1.0f, 0.0f};

private:
    glm::vec3 _localPosition{0.0f};
    glm::vec3 _localRotation{0.0f};
    glm::vec3 _localScale{1.0f};

	glm::quat _cachedQuat;
	glm::mat4 _cachedLocalMatrix;
	glm::mat4 _cachedWorldMatrix;

	bool _cacheDirty = true;
	bool _updatedThisFrame;

	void _UpdateQuaternion();
	void _UpdateLocalMatrix();
	void _UpdateWorldMatrix(const glm::mat4& parentWorldMatrix);
};
