#include "HierarchyPanel.h"
#include "EditorScene.h"
#include "game/Scene.h"

void HierarchyPanel::FromJSON(const json& data) { }

void HierarchyPanel::OnCreate(std::weak_ptr<Scene> scene) {
	_controller.From(_gameObject);
	_scene = scene;
	_BuildUI();
}

void HierarchyPanel::_BuildUI() {
	auto scene = _scene.lock();
	if (!scene) {
		return;
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
				{ "color", { 0.2, 0.2, 0.6, 1 } },
				{ "flow", "Vertical" },
				{ "gap", 5 }
			}}
		}}
	};
	GameObject* go = scene->Instantiate(panel, _gameObject.lock().get());

	for (const auto& filePath : EditorScene::Instance().GetObjectPaths()) {
		const json& doc = EditorScene::Instance().GetDocument(filePath);
		if (doc.empty()) {
			continue;
		}
		_BuildNode(scene, doc, filePath, "", go);
	}
}

void HierarchyPanel::_BuildNode(std::shared_ptr<Scene> scene, const json& node, const std::string& filePath, const std::string& nodePath, GameObject* parent) {
	std::string name = node["name"].get<std::string>();
	std::string fullNodePath = nodePath.empty() ? name : nodePath + "/" + name;

	json chunk = {
		{ "type", "GameObject" },
		{ "name", name },
		{ "properties", {
			{ "UILayout", {
				{ "width", { { "mode", "Percent" }, { "value", 100 } } },
				{ "height", { { "mode", "Auto" } } }
			}},
			{ "UIButton", {
				{ "onClick", {
					{ "key", "hierarchy/select" },
					{ "args", {
						{ "file", filePath },
						{ "node", fullNodePath }
					}}
				}}
			}},
			{ "HierarchyNode", {
				{ "filePath", filePath },
				{ "nodePath", fullNodePath }
			}}
		}},
		{ "children", {
			{
				{ "type", "GameObject" },
				{ "name", "container" },
				{ "properties", {
					{ "UILayout", {
						{ "width", { { "mode", "Percent" }, { "value", 100 } } },
						{ "height", { { "mode", "Auto" } } }
					}},
					{ "UIPanel", {
						{ "color", { 0, 0, 0, 0 } },
						{ "flow", "Depth" }
					}}
				}},
				{ "children", {
					{
						{ "type", "GameObject" },
						{ "name", "bg" },
						{ "properties", {
							{ "UILayout", {
								{ "width", { { "mode", "Percent" }, { "value", 100 } } },
								{ "height", { { "mode", "Percent" }, { "value", 100 } } }
							}},
							{ "UIPanel", {
								{ "color", { 0.2, 0.2, 0.2, 1.0 } }
							}},
						}}
					},
					{
						{ "type", "GameObject" },
						{ "name", "label" },
						{ "properties", {
							{ "UILayout", {
								{ "width", { { "mode", "Auto" } } },
								{ "height", { { "mode", "Auto" } } },
							}},
							{ "UILabel", {
								{ "color", { 1, 1, 1, 1 } },
								{ "text", name },
								{ "font", "assets/textures/fonts/atlas" },
								{ "fontSize", 60 },
								{ "hAlign", "Start" }
							}}
						}}
					}
				}}
			}
		}}
	};

	GameObject* go = scene->Instantiate(chunk, parent);

	if (node.contains("children")) {
		for (const auto& child : node["children"]) {
			_BuildNode(scene, child, filePath, fullNodePath, go);
		}
	}
}

REGISTER_PROPERTY(HierarchyPanel)
