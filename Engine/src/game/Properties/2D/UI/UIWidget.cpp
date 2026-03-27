#include "UIWidget.h"

#include "game/GameObject.h"
#include "game/Properties/2D/UI/UICanvas.h"

void UIWidget::FromJSON(const json& j) {
	_zIndex = j.value("zIndex", 0);
}

void UIWidget::OnCreate(std::weak_ptr<Scene> scene) {
	_camera = &scene.lock()->GetCamera();
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

void UIWidget::Arrange(Rect availableRect) {
	const Sides& margin = _layout->GetMargin();
	Rect margined = {
		availableRect.x + margin.left,
		availableRect.y - margin.top,
		availableRect.width - margin.left - margin.right,
		availableRect.height - margin.top - margin.bottom
	};
	glm::vec2 measured = _MeasureContent();
	float w = _ResolveAxis(_layout->GetWidth(), margined.width, measured.x);
	float h = _ResolveAxis(_layout->GetHeight(), margined.height, measured.y);
	_layout->GetComputedRect() = { margined.x, margined.y, w, h };
	_Arrange();
}

glm::vec2 UIWidget::_MeasureContent() {
	return glm::vec2({ 0, 0 });
}

float UIWidget::_ResolveAxis(Size size, float available, float measured) {
	switch (size.mode) {
		case Size::Mode::Pixels: return size.value;
		case Size::Mode::Percent: return available * (size.value / 100.0f);
		case Size::Mode::Auto: return measured;
	}
	std::cerr << "[UIWidget] Invalid size mode." << std::endl;
	std::abort();
}
