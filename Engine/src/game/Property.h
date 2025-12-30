#pragma once
#include "core/Window.h"
#include "util/Registry.h"
#include <memory>
#include <json.hpp>

using json = nlohmann::json;
class GameObject;
class Renderer;
class Scene;

class Property : public std::enable_shared_from_this<Property>{
public:
    Property() = default;
    virtual ~Property() = default;
    virtual void FromJSON(const json &data, std::weak_ptr<Scene> scene) {}
    virtual void OnCreate() {}
    virtual void Update(float dt) {}
    virtual void OnDestroy() {}
    virtual void HandleInput(const Window& window, float dt) {}
    virtual void Render(Renderer& renderer) {}
	void SetParent(std::weak_ptr<GameObject> parent) {
		_parent = parent;
	}
	std::weak_ptr<GameObject> GetParent() {
		return _parent;
	}

protected:
    std::weak_ptr<GameObject> _parent;
};

#define REGISTER_PROPERTY(DerivedType) REGISTER_TYPE(Property, DerivedType)
