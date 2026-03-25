#pragma once
#include "core/Window.h"
#include "util/Registry.h"
#include <memory>
#include <json.hpp>

using json = nlohmann::json;
class GameObject;
class Renderer;
class Scene;

class Property : public std::enable_shared_from_this<Property> {
public:
    Property() = default;
    virtual ~Property() = default;
    virtual void FromJSON(const json &data) {}
    virtual void OnCreate(std::weak_ptr<Scene> scene) {}
    virtual void Update(float dt) {}
    virtual void OnDestroy() {}
    virtual void HandleInput(const Window& window, float dt) {}
    virtual void Render(Renderer& renderer) {}
	virtual void OnResize(int width, int height) {}
	void SetGameObject(std::weak_ptr<GameObject> gameObject) {
		_gameObject = gameObject;
	}
	std::weak_ptr<GameObject> GetGameObject() {
		return _gameObject;
	}

protected:
    std::weak_ptr<GameObject> _gameObject;
};

#define REGISTER_PROPERTY(DerivedType) REGISTER_TYPE(Property, DerivedType)
