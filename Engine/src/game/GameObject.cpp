#include "GameObject.h"
#include "util/Registry.h"

void GameObject::FromJSON(const json& data) {
    for (auto& propData : data["properties"]) {
        std::string type = propData["type"];
        const json& payload = propData["data"];

        auto property = Registry<Property>::Instance().Create(type);
        if (!property) {
            std::cerr << "Unknown property type: " << type << "\n";
            continue;
        }

        property->FromJSON(payload);
        AddProperty(std::move(property));
    }
}

void GameObject::AddProperty(std::unique_ptr<Property> property) {
    _properties.push_back(std::move(property));
}

void GameObject::OnCreate() {
    for (auto& prop : _properties) {
        prop->OnCreate();
    }
    for (auto& script : _scripts) {
        script->Start();
    }
    for (auto& child : _children) {
        child->OnCreate();
    }
}

void GameObject::Update(float dt) {
    for (auto& prop : _properties) {
        prop->Update(dt);
    }
    for (auto& script : _scripts) {
        script->Update(dt);
    }
    for (auto& child : _children) {
        child->Update(dt);
    }
}

void GameObject::HandleInput(const Window& window, float dt) {
    for (auto& prop : _properties) {
        prop->HandleInput(window, dt);
    }
    for (auto& script : _scripts) {
        script->HandleInput(window, dt);
    }
    for (auto& child : _children) {
        child->HandleInput(window, dt);
    }
}

void GameObject::Render(Renderer& renderer) {
    for (auto& prop : _properties) {
        prop->Render(renderer);
    }
    for (auto& script : _scripts) {
        script->Render(renderer);
    }
    for (auto& child : _children) {
        child->Render(renderer);
    }
}

void GameObject::OnDestroy() {
    for (auto& prop : _properties) {
        prop->OnDestroy();
    }
    for (auto& script : _scripts) {
        script->Destroy();
    }
    for (auto& child : _children) {
        child->OnDestroy();
    }
}