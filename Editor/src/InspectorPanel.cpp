#include "InspectorPanel.h"
#include "game/Scene.h"
#include "EditorScene.h"
#include "game/Properties/2D/UI/UIButton.h"
#include <iostream>

void InspectorPanel::FromJSON(const json& data) { }

void InspectorPanel::OnCreate(std::weak_ptr<Scene> scene) {
	_scene = scene;

	auto s = _scene.lock();
	if (!s) {
		return;
	}

	auto controllerObj = s->FindByPath("hierarchy");
	if (!controllerObj) {
		return;
	}

	_controller = controllerObj->GetProperty<HierarchyController>();
	if (auto controller = _controller.lock()) {
		_subscriptionId = controller->onSelectionChanged.Subscribe([this]() {
			_dirty = true;
		});
	}

	_Rebuild();
}

void InspectorPanel::OnDestroy() {
	if (auto controller = _controller.lock()) {
		controller->onSelectionChanged.Unsubscribe(_subscriptionId);
	}
}

void InspectorPanel::Update(float dt) {
	if (_dirty) {
		_Rebuild();
		_dirty = false;
	}
}

void InspectorPanel::_Rebuild() {
	auto scene = _scene.lock();
	if (!scene) {
		return;
	}

	auto self = _gameObject.lock();
	if (!self) {
		return;
	}

	GameObject* oldPanelGO = self->GetChildByName("panel");
	if (oldPanelGO) {
		oldPanelGO->OnDestroy();
	}

	json panel = {
		{ "type", "GameObject" },
		{ "name", "panel" },
		{ "properties", {
			{ "UILayout", {
				{ "width", { { "mode", "Percent" }, { "value", 100 } } },
				{ "height", { { "mode", "Percent" }, { "value", 100 } } },
			}},
			{ "UIPanel", {
				{ "color", { 0.1, 0.1, 0.4, 1 } },
				{ "flow", "Vertical" },
				{ "gap", 5 }
			}}
		}}
	};
	GameObject* panelGO = scene->Instantiate(panel, self.get());

	if (auto controller = _controller.lock()) {
		const std::string& filePath = controller->GetSelectedFile();
		const std::string& nodePath = controller->GetSelectedNode();
	
		if (filePath.empty() || nodePath.empty()) {
			return;
		}
	
		const json* node = EditorScene::Instance().GetNodeAtPath(filePath, nodePath);
		if (!node) {
			return;
		}
	
		for (const auto& [key, value] : node->items()) {
			json header = {
				{ "type", "GameObject" },
				{ "name", key + "_header" },
				{ "properties", {
					{ "UILayout", {
						{ "width", { { "mode", "Percent" }, { "value", 100 } } },
						{ "height", { { "mode", "Auto" } } }
					}},
					{ "UIButton", {
						{ "_placeholder", true }
					}},
				}},
				{ "children", {
					{
						{ "type", "GameObject" },
						{ "name", "buttonLabel" },
						{ "properties", {
							{ "UILayout", {
								{ "width", { { "mode", "Percent" }, { "value", 100 } } },
								{ "height", { { "mode", "Auto" } } }
							}},
							{ "UILabel", {
								{ "color", { 1, 1, 1, 1 } },
								{ "text", key },
								{ "font", "assets/textures/fonts/atlas" },
								{ "fontSize", 20 },
								{ "hAlign", "Start" }
							}}
						}}
					}
				}}
			};

			json content = {
				{ "type", "GameObject" },
				{ "name", key + "_content" },
				{ "properties", {
					{ "UILayout", {
						{ "width", { { "mode", "Percent" }, { "value", 100 } } },
						{ "height", { { "mode", "Auto" } } },
					}},
					{ "UIPanel", {
						{ "color", { 0.15, 0.15, 0.45, 1 } },
						{ "flow", "Vertical" },
						{ "gap", 2 }
					}},
					{ "UILabel", {
						{ "color", { 0.8, 0.8, 0.8, 1 } },
						{ "text", value.dump() },
						{ "font", "assets/textures/fonts/atlas" },
						{ "fontSize", 16 },
						{ "hAlign", "Start" }
					}}
				}}
			};

			scene->Instantiate(header, panelGO);
			GameObject* contentGO = scene->Instantiate(content, panelGO);

			GameObject* headerGO = panelGO->GetChildByName(key + "_header");
			if (headerGO) {
				UIButton* button = headerGO->GetProperty<UIButton>().get();
				if (button) {
					std::cout << "found a button" << std::endl;
					button->onClick.Subscribe([contentGO]() {
						std::cout << "click!" << std::endl;
						contentGO->SetActive(!contentGO->GetActive());
					});
				}
			}
		}
	}
}

REGISTER_PROPERTY(InspectorPanel)
