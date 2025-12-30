#include "GameObject.h"
#include "util/Registry.h"
#include "game/Scene.h"
#include <iostream>

void GameObject::FromJSON(const json& data, std::weak_ptr<Scene> scene) {
    for (auto& propData : data["properties"]) {
        std::string type = propData["type"];
        const json& payload = propData["data"];

        auto property = Registry<Property>::Instance().Create(type);
        if (!property) {
            std::cerr << "Unknown property type: " << type << "\n";
            continue;
        }

		property->SetParent(shared_from_this());
        AddProperty(property);

        property->FromJSON(payload, scene);
    }
	if (data.contains("children")) {
		for (auto& childData : data["children"]) {
			auto child = std::make_shared<GameObject>();
			child->FromJSON(childData, scene);
			child->_parent = shared_from_this();
			_children.push_back(std::move(child));
		}
	}
}

void GameObject::AddProperty(std::shared_ptr<Property> property) {
    _properties.push_back(property);
}

void GameObject::OnCreate() {
    for (auto& prop : _properties) {
        prop->OnCreate();
    }
    for (auto& child : _children) {
        child->OnCreate();
    }
}

void GameObject::Update(float dt) {
    for (auto& prop : _properties) {
        prop->Update(dt);
    }
    for (auto& child : _children) {
        child->Update(dt);
    }
}

void GameObject::HandleInput(const Window& window, float dt) {
    for (auto& prop : _properties) {
        prop->HandleInput(window, dt);
    }
    for (auto& child : _children) {
        child->HandleInput(window, dt);
    }
}

void GameObject::Render(Renderer& renderer) {
    for (auto& prop : _properties) {
        prop->Render(renderer);
    }
    for (auto& child : _children) {
        child->Render(renderer);
    }
}

void GameObject::OnDestroy() {
    for (auto& prop : _properties) {
        prop->OnDestroy();
    }
    for (auto& child : _children) {
        child->OnDestroy();
    }
}
