#include "UIWidget.h"

#include "game/GameObject.h"
#include "game/Properties/2D/UI/UICanvas.h"
#include "UIContainer.h"

void UIWidget::FromJSON(const json& j) {
	_zIndex = j.value("zIndex", 0);
}

void UIWidget::OnCreate(std::weak_ptr<Scene> scene) {
	std::cout << "creating UIWidget: " << _gameObject.lock()->GetName() << std::endl;
	_camera = &scene.lock()->GetCamera();
	_layout.From(_gameObject);

	std::weak_ptr<GameObject> parent = _gameObject.lock()->GetParent();
	while (std::shared_ptr<GameObject> parentObj = parent.lock()) {
		if (std::shared_ptr<UIContainer> widget = parentObj->GetProperty<UIContainer>()) {
			_parent = widget;
			widget->AddChild(std::static_pointer_cast<UIWidget>(shared_from_this()));
			return;
		}
		if (std::shared_ptr<UICanvas> canvas = parentObj->GetProperty<UICanvas>()) {
			canvas->RegisterWidget(std::static_pointer_cast<UIWidget>(shared_from_this()));
			return;
		}
		parent = parentObj->GetParent();
	}
	std::cerr << "[UIWidget] No UICanvas found in parent chain." << std::endl;
}

void UIWidget::OnDestroy() {
	if (auto parent = _gameObject.lock()->GetParent().lock()) {
		if (auto container = parent->GetProperty<UIContainer>()) {
			container->RemoveChild(this);
		}
		if (auto canvas = parent->GetProperty<UICanvas>()) {
			canvas->UnregisterWidget(this);
		}
	}
}

UILayout* UIWidget::GetLayout() {
	return _layout.Get();
}

int UIWidget::GetZIndex() const {
	return _zIndex;
}

void UIWidget::MarkDirty() {
	_dirty = true;
	if (auto parent = _parent.lock()) {
		parent->MarkDirty();
	}
}

void UIWidget::Arrange(Rect availableRect) {
	if (!_gameObject.lock()->GetActive()) {
		return;
	}
	if (!_dirty) {
		return;
	}
	const Sides& margin = _layout->GetMargin();
	Rect margined = {
		availableRect.x + margin.left,
		availableRect.y - margin.top,
		availableRect.width - margin.left - margin.right,
		availableRect.height - margin.top - margin.bottom
	};
	glm::vec2 measured = MeasureContent();
	float w = _ResolveAxis(_layout->GetWidth(), margined.width, measured.x);
	float h = _ResolveAxis(_layout->GetHeight(), margined.height, measured.y);
	_layout->GetComputedRect() = { margined.x, margined.y, w, h };
	_Arrange();
	_dirty = false;
}

glm::vec2 UIWidget::MeasureContent() {
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
