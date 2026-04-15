#include "InspectorPanel.h"
#include "game/Scene.h"
#include "EditorScene.h"
#include "game/Properties/2D/UI/UIInput.h"

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
			json dropdown = {
			    { "type", "GameObject" },
			    { "name", key },
			    { "properties", {
			        { "UILayout", {
			            { "width", { { "mode", "Percent" }, { "value", 100 } } },
			            { "height", { { "mode", "Auto" } } }
			        }},
			        { "UIDropdown", {
			            { "open", false },
			            { "bar", {
			                { "type", "GameObject" },
			                { "name", "bar" },
			                { "properties", {
			                    { "UILayout", {
			                        { "width", { { "mode", "Percent" }, { "value", 100 } } },
			                        { "height", { { "mode", "Auto" } } }
			                    }},
			                    { "UIButton", {
									{ "_placeholder", true }
								}}
			                }},
			                { "children", {{
			                    { "type", "GameObject" },
			                    { "name", "label" },
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
			                }}}
			            }},
			            { "body", {
			                { "type", "GameObject" },
			                { "name", "body" },
			                { "properties", {
			                    { "UILayout", {
			                        { "width", { { "mode", "Percent" }, { "value", 100 } } },
			                        { "height", { { "mode", "Auto" } } }
			                    }},
			                    { "UIPanel", {
			                        { "color", { 0.15, 0.15, 0.45, 1 } },
			                        { "flow", "Vertical" },
			                        { "gap", 2 }
			                    }}
			                }},
			                { "children", {{
								{ "type", "GameObject" },
								{ "name", "value" },
								{ "properties", {
								    { "UILayout", {
								        { "width", { { "mode", "Percent" }, { "value", 100 } } },
								        { "height", { { "mode", "Auto" } } }
								    }},
								    { "UIInput", {
								        { "text", value.dump() },
								        { "font", "assets/textures/fonts/atlas" },
								        { "fontSize", 16 },
								        { "textAlign", "Start" }
								    }}
								}}
			                }}}
			            }}
			        }}
			    }}
			};
			GameObject* dropdownGO = scene->Instantiate(dropdown, panelGO);
			GameObject* valueGO = dropdownGO->GetChildByPath("body/value");
			if (valueGO) {
				if (auto input = valueGO->GetProperty<UIInput>()) {
					input->onCommit.Subscribe([filePath, nodePath, key](const std::string& newVal) {
						json* node = EditorScene::Instance().GetNodeAtPath(filePath, nodePath);
						if (node) {
							try {
								(*node)[key] = json::parse(newVal);
							} catch (const json::parse_error& e) {
								std::cerr << "Invalid JSON for key '" << key << "': " << e.what() << std::endl;
							}
						}
					});
				}
			}
		}
	}
}

REGISTER_PROPERTY(InspectorPanel)
