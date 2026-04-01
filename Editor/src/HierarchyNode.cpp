#include "HierarchyNode.h"
#include "game/Property.h"
#include "game/GameObject.h"
#include "game/Properties/2D/UI/UIPanel.h"

void HierarchyNode::FromJSON(const json& data) {
	_filePath = data["filePath"];
	_nodePath = data["nodePath"];
}

void HierarchyNode::OnCreate(std::weak_ptr<Scene> scene) {
	
}

void HierarchyNode::Select() {
	GameObject* bgGO = _gameObject.lock()->GetChildByPath("container/bg");
	if (!bgGO) {
		return;
	}
	
	UIPanel* bgP = bgGO->GetProperty<UIPanel>();
	if (!bgP) {
		return;
	}

	bgP->SetColor(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
}

void HierarchyNode::Deselect() {
	GameObject* bgGO = _gameObject.lock()->GetChildByPath("container/bg");
	if (!bgGO) {
		return;
	}
	
	UIPanel* bgP = bgGO->GetProperty<UIPanel>();
	if (!bgP) {
		return;
	}

	bgP->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
}

REGISTER_PROPERTY(HierarchyNode)
