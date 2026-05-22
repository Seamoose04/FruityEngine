#include "UIDropdown.h"
#include "game/Property.h"
#include "game/Scene.h"

void UIDropdown::FromJSON(const json& j) {
	UIContainer::FromJSON(j);
	_open = j.value("open", false);
	_barChunk = j["bar"];
	_bodyChunk = j["body"];
}

void UIDropdown::OnCreate(std::weak_ptr<Scene> weakScene) {
	UIWidget::OnCreate(weakScene);

	auto scene = weakScene.lock();
	if (!scene) {
		return;
	}
	
	_bar = scene->Instantiate(_barChunk, _gameObject.lock().get())->GetProperty<UIButton>();
	_body = scene->Instantiate(_bodyChunk, _gameObject.lock().get())->GetProperty<UIContainer>();
	_body->GetGameObject().lock()->SetActive(_open);

	_bar->onClick.Subscribe([this]() {
		_open = !_open;
		_body->GetGameObject().lock()->SetActive(_open);
		MarkDirty();
	});
}

glm::vec2 UIDropdown::MeasureContent() {
	glm::vec2 barSize = _bar->MeasureContent();
	glm::vec2 bodySize = _body->MeasureContent();
	return _open ? glm::vec2(std::max(barSize.x, bodySize.x), barSize.y + bodySize.y) : barSize;
}

void UIDropdown::_Arrange() {
	const Rect& computed = _layout->GetComputedRect();
	Rect inner = {
		computed.x + _padding.left,
		computed.y - _padding.top,
		computed.width - _padding.left - _padding.right,
		computed.height - _padding.top - _padding.bottom
	};

	_bar->SetDirty();
	_bar->Arrange(inner);
	float barHeight = _bar->GetLayout()->GetComputedRect().height;

	if (_open) {
		Rect bodyRect = inner;
		bodyRect.y -= barHeight;
		bodyRect.height = inner.height - barHeight;
		_body->SetDirty();
		_body->Arrange(bodyRect);
	}
}

REGISTER_PROPERTY(UIDropdown)
