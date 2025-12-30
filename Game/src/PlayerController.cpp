#include "game/Properties/Transform.h"
#include "game/Property.h"
#include <iostream>
#include "game/PropertyRef.h"

class PlayerController : public Property {
public:
	void FromJSON(const json& data, std::weak_ptr<Scene> scene) override {
		_startMessage = data["startMessage"];
		_speed = data["speed"];
		_mouseSpeed = data["mouseSpeed"];
		_scene = scene;
	}
	void OnCreate() override {
		_transform.From(_parent);
		std::cout << "Player says: " << _startMessage << "\n";
		std::cout << "Raw mouse supported: " << glfwRawMouseMotionSupported() << std::endl;
		if (auto lockedScene = _scene.lock()) {
			lockedScene->SetFlag(SceneFlags::CursorLocked);
			lockedScene->SetFlag(SceneFlags::CursorHidden);
		}
		std::cout << "SceneFlags::Cursor set" << std::endl;
	}
    void HandleInput(const Window &window, float dt) override {
		if (window.IsKeyPressed(GLFW_KEY_W)) {
			_transform->position += _transform->Forward() * dt * _speed;
		}
		if (window.IsKeyPressed(GLFW_KEY_S)) {
			_transform->position -= _transform->Forward() * dt * _speed;
		}
		if (window.IsKeyPressed(GLFW_KEY_A)) {
			_transform->position -= _transform->Right() * dt * _speed;
		}
		if (window.IsKeyPressed(GLFW_KEY_D)) {
			_transform->position += _transform->Right() * dt * _speed;
		}
		if (window.IsKeyPressed(GLFW_KEY_Q)) {
			if (auto lockedScene = _scene.lock()) {
				lockedScene->SetFlag(SceneFlags::Quit);
			}
		}

		glm::vec2 mousePos = window.GetMousePos();
 		auto &mutatableWin = const_cast<Window&>(window);
	 	// mutatableWin.ResetMousePos();
		// std::cout << "reset mouse" << std::endl;

		_transform->SetRotation(glm::vec3(0, mousePos.x * _mouseSpeed, 0));
		// _transform->Rotate(glm::vec3(0, mousePos.x * _mouseSpeed, 0));
	}

private:
	std::weak_ptr<Scene> _scene;
	std::string _startMessage;
	float _speed;
	float _mouseSpeed;
	PropertyRef<Transform> _transform;
};

REGISTER_PROPERTY(PlayerController)
