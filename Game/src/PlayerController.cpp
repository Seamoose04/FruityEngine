#include "game/Properties/Transform.h"
#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Scene.h"

class PlayerController : public Property {
public:
	void FromJSON(const json& data) override {
		_speed = data["speed"];
	}
	void OnCreate(std::weak_ptr<Scene> scene) override {
		_scene = scene;
		_transform.From(_parent);
		if (auto lockedScene = _scene.lock()) {
			lockedScene->SetFlag(SceneFlags::VNC);
			lockedScene->SetFlag(SceneFlags::CursorHidden);
		}
	}
    void HandleInput(const Window &window, float dt) override {
		if (window.IsKeyPressed(GLFW_KEY_Q)) {
			if (auto lockedScene = _scene.lock()) {
				lockedScene->SetFlag(SceneFlags::Quit);
			}
		}
	}

private:
	std::weak_ptr<Scene> _scene;
	float _speed;
	PropertyRef<Transform> _transform;
};

REGISTER_PROPERTY(PlayerController)
