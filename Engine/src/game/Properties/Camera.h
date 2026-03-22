#pragma once
#include <glm/glm.hpp>
#include "game/Properties/Transform.h"
#include "game/PropertyRef.h"
#include "game/Property.h"

class Camera : public Property {
public:
	enum class ProjectionMode {
		Perspective,
		Orthographic
	};
    void FromJSON(const json& data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Render(Renderer& renderer) override;
	void UpdateAspectRatio(int width, int height);
	void UpdateView();
	void UpdateProjection();
	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;

private:
	float _fov;
	float _near;
	float _far;
	float _aspectRatio = 1920.0f / 1080.0f;
	int _screenWidth = 1920;
	int _screenHeight = 1080;

	std::weak_ptr<Scene> _scene;
    glm::mat4 _view;
    glm::mat4 _projection;
	PropertyRef<Transform> _transform;
	ProjectionMode _mode;
};

