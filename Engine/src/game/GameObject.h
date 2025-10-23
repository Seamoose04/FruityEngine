#pragma once
#include "core/Window.h"
#include "graphics/Renderer.h"
#include "game/Property.h"
#include "util/Registry.h"
#include "game/Script.h"
#include <vector>
#include <memory>

class GameObject {
public:
    GameObject() = default;
    ~GameObject() = default;

    virtual void OnCreate();
    virtual void Update(float dt);
    virtual void OnDestroy();
    virtual void HandleInput(const Window& window, float dt);
    virtual void Render(Renderer& renderer);

    void FromJSON(const json& data);
    void AddProperty(std::unique_ptr<Property> property);

    template<typename T>
    T* GetProperty();

    template <typename T, typename... Args>
    T* AddScript(Args&&... args);
    
private:
    std::vector<std::unique_ptr<Property>> _properties;
    std::vector<std::unique_ptr<Script>> _scripts;
    std::weak_ptr<GameObject> _parent;
    std::vector<std::shared_ptr<GameObject>> _children;
};

template<typename T>
T* GameObject::GetProperty() {
    for (auto& p : _properties) {
        if (auto casted = dynamic_cast<T*>(p.get()))
            return casted;
    }
    return nullptr;
}

template <typename T, typename... Args>
T* GameObject::AddScript(Args&&... args) {
    static_assert(std::is_base_of<Script, T>::value, "T must derive from Script");
    auto script = std::make_unique<T>(std::forward<Args>(args)...);
    script->gameObject = this;
    T* ptr = script.get();
    scripts.push_back(std::move(script));
    return ptr;
}