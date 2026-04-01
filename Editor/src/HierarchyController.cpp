#include "HierarchyController.h"
#include "game/ActionRegistry.h"
#include "game/GameObject.h"
#include "HierarchyNode.h"

void HierarchyController::FromJSON(const json& data) { }

void HierarchyController::OnCreate(std::weak_ptr<Scene> scene) {
	ActionRegistry::Instance().Register("hierarchy", "select", [this](const json& args) {
		std::cout << "Selected '" << args["node"].get<std::string>() << "'" << std::endl;
		_SelectNode(args["file"], args["node"]);
	});
}
void HierarchyController::OnDestroy() {
	ActionRegistry::Instance().Unregister("hierarchy", "select");
}

const std::string& HierarchyController::GetSelectedNode() const {
	return _selectedNode;
}

const std::string& HierarchyController::GetSelectedFile() const {
	return _selectedFile;
}

void HierarchyController::_SelectNode(const std::string& file, const std::string& node) {
	GameObject* pNodeGO = _gameObject.lock()->GetChildByPath("panel/" + _selectedNode);
	if (pNodeGO) {
		HierarchyNode* nodeP = pNodeGO->GetProperty<HierarchyNode>();
		if (nodeP) {
			nodeP->Deselect();
		}
	}
	_selectedFile = file;
	_selectedNode = node;

	GameObject* nodeGO = _gameObject.lock()->GetChildByPath("panel/" + _selectedNode);
	if (nodeGO) {
		HierarchyNode* nodeP = nodeGO->GetProperty<HierarchyNode>();
		if (nodeP) {
			nodeP->Select();
		}
	}
}

REGISTER_PROPERTY(HierarchyController)
