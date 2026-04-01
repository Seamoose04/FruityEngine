#include "game/Property.h"
#include "HierarchyController.h"
#include "game/PropertyRef.h"

class HierarchyPanel : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;

private:
	void _BuildUI();
	void _BuildNode(std::shared_ptr<Scene>, const json& node, const std::string& filePath, const std::string& nodePath, GameObject* parent);

	std::weak_ptr<Scene> _scene;
	PropertyRef<HierarchyController> _controller;
};
