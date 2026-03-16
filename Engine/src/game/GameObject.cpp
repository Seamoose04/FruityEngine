#include "GameObject.h"
#include "util/Registry.h"
#include "game/Scene.h"
#include <iostream>
#include <memory>

void GameObject::FromJSON(const json& data) {
    for (auto& [key, value] : data["properties"].items()) {
        auto property = Registry<Property>::Instance().Create(key);
        if (!property) {
            std::cerr << "Unknown property type: " << key << "\n";
            continue;
        }

		property->SetParent(shared_from_this());
        AddProperty(property);

        property->FromJSON(value);
	}
	if (data.contains("children")) {
		for (auto& childData : data["children"]) {
			auto child = std::make_shared<GameObject>();
			child->FromJSON(childData);
			child->_parent = shared_from_this();
			_children.push_back(std::move(child));
		}
	}
}

void GameObject::AddProperty(std::shared_ptr<Property> property) {
    _properties.push_back(property);
}

void GameObject::OnCreate(std::weak_ptr<Scene> scene) {
    for (auto& prop : _properties) {
        prop->OnCreate(scene);
    }
    for (auto& child : _children) {
        child->OnCreate(scene);
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

const std::weak_ptr<GameObject>& GameObject::GetParent() const {
	return _parent;
}

const std::vector<std::shared_ptr<GameObject>>& GameObject::GetChildren() const {
	return _children;
}
