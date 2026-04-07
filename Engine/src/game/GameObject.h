#pragma once
#include "core/Window.h"
#include "graphics/Renderer.h"
#include "game/Property.h"
#include <string>
#include <vector>
#include <memory>

class Scene;
class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject() = default;
	~GameObject() = default;

	void OnCreate(std::weak_ptr<Scene> scene);
	void Update(float dt);
	void OnDestroy();
	void OnResize(int width, int height);
	void HandleInput(const Window &window, float dt);
	void Render(Renderer &renderer);

	void FromJSON(const json &data);
	void AddProperty(std::shared_ptr<Property> property);

	const std::weak_ptr<GameObject>& GetParent() const;
	const std::vector<std::shared_ptr<GameObject>>& GetChildren() const;
	void AddChild(std::shared_ptr<GameObject> child);
	GameObject* GetChildByName(const std::string& name) const;
	GameObject* GetChildByPath(const std::string& path) const;

	const bool& GetActive() const;
	void SetActive(bool active);

	const std::string& GetName() const;
	template <typename T>
	std::shared_ptr<T> GetProperty();

private:
	std::vector<std::shared_ptr<Property>> _properties;
	std::weak_ptr<GameObject> _parent;
	std::vector<std::shared_ptr<GameObject>> _children;
	std::string _name;
	bool _active;
};

template <typename T>
std::shared_ptr<T> GameObject::GetProperty() {
	for (auto &p : _properties) {
		if (auto casted = dynamic_pointer_cast<T>(p)) {
			return casted;
		}
	}
	return nullptr;
}
