#pragma once
#include "core/Window.h"
#include "graphics/Renderer.h"
#include <vector>
#include <memory>
#include <json.hpp>

using json = nlohmann::json;
class GameObject;

class Property {
public:
    Property() = default;
    virtual ~Property() = default;
    virtual void FromJSON(const json& data) {}
    virtual void OnCreate() {}
    virtual void Update(float dt) {}
    virtual void OnDestroy() {}
    virtual void HandleInput(const Window& window, float dt) {}
    virtual void Render(Renderer& renderer) {}

protected:
    std::weak_ptr<GameObject> _parent;
};

#define REGISTER_PROPERTY(DerivedType) REGISTER_TYPE(Property, DerivedType)