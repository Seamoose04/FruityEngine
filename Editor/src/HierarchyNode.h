#pragma once

#include "game/Property.h"

class HierarchyNode : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void Select();
	void Deselect();

private:
	std::string _filePath;
	std::string _nodePath;
};
