#pragma once
#include "core/Window.h"
#include "graphics/Renderer.h"
#include "game/Property.h"
#include <vector>
#include <memory>

class Scene;
class GameObject : public std::enable_shared_from_this<GameObject> {
public:
	GameObject() = default;
	~GameObject() = default;

	void OnCreate();
	void Update(float dt);
	void OnDestroy();
	void HandleInput(const Window &window, float dt);
	void Render(Renderer &renderer);

	void FromJSON(const json &data, std::weak_ptr<Scene> scene);
	void AddProperty(std::shared_ptr<Property> property);

	template <typename T>
	T *GetProperty();

private:
	std::vector<std::shared_ptr<Property>> _properties;
	std::weak_ptr<GameObject> _parent;
	std::vector<std::shared_ptr<GameObject>> _children;
};

template <typename T>
T *GameObject::GetProperty() {
	for (auto &p : _properties) {
		if (auto casted = dynamic_cast<T *>(p.get())) {
			return casted;
		}
	}
	return nullptr;
}
