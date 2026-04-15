#include "UIContainer.h"

void UIContainer::FromJSON(const json& j) {
	UIWidget::FromJSON(j);
	if (j.contains("padding")) {
		_padding = Sides::FromJSON(j["padding"]);
	} else {
		_padding = { 0.0f, 0.0f, 0.0f, 0.0f };
	}
}

void UIContainer::AddChild(const std::shared_ptr<UIWidget> child) {
	_children.push_back(child);
	MarkDirty();
}

std::vector<UIWidget*> UIContainer::ActiveChildren() const {
	std::vector<UIWidget*> result;
	for (auto& child : _children) {
		if (auto go = child->GetGameObject().lock()) {
			if (go->GetActive()) {
				result.push_back(child.get());
			}
		}
	}
	return result;
}

void UIContainer::SetPadding(const Sides& padding) {
	_padding = padding;
}

const Sides& UIContainer::GetPadding() const {
	return _padding;
}

void UIContainer::Draw(Renderer& renderer) {
	for (auto& child : ActiveChildren()) {
        child->Draw(renderer);
    }
}

void UIContainer::RemoveChild(UIWidget* child) {
	_children.erase(
		std::remove_if(_children.begin(), _children.end(),
			[child](const std::shared_ptr<UIWidget> c) {
			return c.get() == child;
		}),
		_children.end()
	);
	MarkDirty();
}

void UIContainer::OnDestroy() {
	UIWidget::OnDestroy();
	_children.clear();
}

void UIContainer::_Arrange() {
	for (auto& child : ActiveChildren()) {
		child->SetDirty();
	}
}
