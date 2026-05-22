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
				{ "color", { 0.08, 0.08, 0.12, 1.0 } },
				{ "flow", "Vertical" },
				{ "gap", 4 },
				{ "padding", { { "top", 6 }, { "bottom", 6 }, { "left", 6 }, { "right", 6 } } }
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
									{ "height", { { "mode", "Pixels" }, { "value", 28 } } }
								}},
								{ "UIButton", {
									{ "_placeholder", true }
								}}
							}},
							{ "children", {{
								{ "type", "GameObject" },
								{ "name", "bar_content" },
								{ "properties", {
									{ "UILayout", {
										{ "width", { { "mode", "Percent" }, { "value", 100 } } },
										{ "height", { { "mode", "Percent" }, { "value", 100 } } }
									}},
									{ "UIPanel", {
										{ "color", { 0.14, 0.14, 0.22, 1.0 } },
										{ "flow", "Horizontal" },
										{ "gap", 6 },
										{ "padding", { { "top", 4 }, { "bottom", 4 }, { "left", 8 }, { "right", 8 } } }
									}}
								}},
								{ "children", {
									{
										{ "type", "GameObject" },
										{ "name", "indicator" },
										{ "properties", {
											{ "UILayout", {
												{ "width", { { "mode", "Pixels" }, { "value", 3 } } },
												{ "height", { { "mode", "Pixels" }, { "value", 14 } } }
											}},
											{ "UIIcon", {
												{ "color", { 0.0, 0.7, 0.9, 1.0 } }
											}}
										}}
									},
									{
										{ "type", "GameObject" },
										{ "name", "label" },
										{ "properties", {
											{ "UILayout", {
												{ "width", { { "mode", "Auto" } } },
												{ "height", { { "mode", "Auto" } } }
											}},
											{ "UILabel", {
												{ "color", { 0.85, 0.85, 0.9, 1.0 } },
												{ "text", key },
												{ "font", "assets/textures/fonts/atlas" },
												{ "fontSize", 16 },
												{ "hAlign", "Start" }
											}}
										}}
									}
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
									{ "color", { 0.11, 0.11, 0.18, 1.0 } },
									{ "flow", "Vertical" },
									{ "gap", 2 },
									{ "padding", { { "top", 4 }, { "bottom", 4 }, { "left", 16 }, { "right", 4 } } }
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
										{ "fontSize", 14 },
										{ "textAlign", "Start" },
										{ "color", { 0.18, 0.18, 0.25, 1.0 } },
										{ "textColor", { 0.75, 0.75, 0.8, 1.0 } }
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
