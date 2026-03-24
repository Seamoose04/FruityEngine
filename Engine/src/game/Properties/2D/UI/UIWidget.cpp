#include "UIWidget.h"

#include "game/GameObject.h"
#include "game/Properties/2D/UI/UICanvas.h"

void UIWidget::FromJSON(const json& j) {
	_zIndex = j.value("zIndex", 0);
}

void UIWidget::OnCreate(std::weak_ptr<Scene> scene) {
	_layout.From(_gameObject);

	auto parent = _gameObject.lock()->GetParent();
	while (auto parentObj = parent.lock()) {
		if (auto widget = parentObj->GetProperty<UIWidget>()) {
			widget->AddChild(std::static_pointer_cast<UIWidget>(shared_from_this()));
			return;
		}
		if (auto canvas = parentObj->GetProperty<UICanvas>()) {
			canvas->RegisterWidget(this);
			return;
		}
		parent = parentObj->GetParent();
	}
	std::cerr << "[UIWidget] No UICanvas found in parent chain." << std::endl;
}

UILayout* UIWidget::GetLayout() {
	return _layout.Get();
}

int UIWidget::GetZIndex() const {
	return _zIndex;
}

void UIWidget::AddChild(const std::shared_ptr<UIWidget> child) {
	_children.push_back(child);
}
