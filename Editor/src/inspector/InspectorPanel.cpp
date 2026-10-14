#include "InspectorPanel.h"
#include "game/Scene.h"
#include "EditorScene.h"
#include "game/Properties/2D/UI/UIInput.h"
#include "core/FileLoaders/JSONLoader.h"

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

	JSONLoader loader;
	json panel = loader.LoadChunk({{ "$import", "InspectorPanel.json" }});
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
			json item = loader.LoadChunk({
				{ "$import", "InspectorItem.json" },
				{ "$override", {
					{ "name", key }
				}}
			});

			GameObject* itemGO = scene->Instantiate(item, panelGO);
			GameObject* valueGO = itemGO->GetChildByPath("body/value");
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
					input->SetText(value.dump());
				}
			}

			GameObject* labelGO = itemGO->GetChildByPath("bar/bar_content/label");
			if (labelGO) {
				if (auto label = labelGO->GetProperty<UILabel>()) {
					label->SetText(key);
				}
			}
		}
	}
}

REGISTER_PROPERTY(InspectorPanel)
