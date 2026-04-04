#pragma once

#include "game/Property.h"
#include "HierarchyController.h"

class InspectorPanel : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void OnDestroy() override;
	void Update(float dt) override;

private:
	void _Rebuild();

	std::weak_ptr<Scene> _scene;
	std::weak_ptr<HierarchyController> _controller;
	size_t _subscriptionId = 0;
	bool _dirty = false;
};
