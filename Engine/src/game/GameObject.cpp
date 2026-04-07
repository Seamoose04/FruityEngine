#include "GameObject.h"
#include "util/Registry.h"
#include "game/Scene.h"
#include <algorithm>
#include <iostream>
#include <memory>

void GameObject::FromJSON(const json& data) {
	_name = data["name"];
	_active = data.value("active", true);
    for (auto& [key, value] : data["properties"].items()) {
        auto property = Registry<Property>::Instance().Create(key);
        if (!property) {
            std::cerr << "Unknown property type: " << key << "\n";
            continue;
        }

		property->SetGameObject(shared_from_this());
        AddProperty(property);

        property->FromJSON(value);
	}
}

void GameObject::AddProperty(std::shared_ptr<Property> property) {
    _properties.push_back(property);
}

void GameObject::OnCreate(std::weak_ptr<Scene> scene) {
    for (auto& child : _children) {
        child->OnCreate(scene);
    }
    for (auto& prop : _properties) {
        prop->OnCreate(scene);
    }
}

void GameObject::Update(float dt) {
	if (!_active) {
		return;
	}
    for (auto& prop : _properties) {
        prop->Update(dt);
    }
    for (auto& child : _children) {
        child->Update(dt);
    }
}

void GameObject::HandleInput(const Window& window, float dt) {
	if (!_active) {
		return;
	}
    for (auto& prop : _properties) {
        prop->HandleInput(window, dt);
    }
    for (auto& child : _children) {
        child->HandleInput(window, dt);
    }
}

void GameObject::Render(Renderer& renderer) {
	if (!_active) {
		return;
	}
    for (auto& prop : _properties) {
        prop->Render(renderer);
    }
    for (auto& child : _children) {
        child->Render(renderer);
    }
}

void GameObject::OnResize(int width, int height) {
	if (!_active) {
		return;
	}
	for (auto& prop : _properties) {
		prop->OnResize(width, height);
	}
	for (auto& child : _children) {
		child->OnResize(width, height);
	}
}

void GameObject::OnDestroy() {
    for (auto& child : _children) {
		child->_parent.reset();
        child->OnDestroy();
    }
	_children.clear();
    
	for (auto& prop : _properties) {
        prop->OnDestroy();
    }
	_properties.clear();
	
	if (auto parent = _parent.lock()) {
		auto& children = parent->_children;
		children.erase(
			std::remove_if(children.begin(), children.end(),
				[this](std::shared_ptr<GameObject> child) {
					return child.get() == this;
				}),
			children.end()
		);
	}
}

const std::weak_ptr<GameObject>& GameObject::GetParent() const {
	return _parent;
}

const bool& GameObject::GetActive() const {
	return _active;
}

void GameObject::SetActive(bool active) {
	_active = active;
}

const std::vector<std::shared_ptr<GameObject>>& GameObject::GetChildren() const {
	return _children;
}

void GameObject::AddChild(std::shared_ptr<GameObject> child) {
	child->_parent = shared_from_this();
	_children.push_back(child);
}

GameObject* GameObject::GetChildByName(const std::string& name) const {
	auto it = std::find_if(_children.begin(), _children.end(), [&](const std::shared_ptr<GameObject>& child) {
		return child->GetName() == name;
	});
	if (it == _children.end()) {
		return nullptr;
	}
	return it->get();
}

GameObject* GameObject::GetChildByPath(const std::string& path) const {
	size_t slash = path.find('/');
	std::string before = path.substr(0, slash);
	std::string after = (slash != std::string::npos) ? path.substr(slash + 1) : "";

	GameObject* child = GetChildByName(before);
	if (!child || after.empty()) {
		return child;
	}
	return child->GetChildByPath(after);
}

const std::string& GameObject::GetName() const {
	return _name;
}
