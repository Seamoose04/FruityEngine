#include "UIContainer.h"

void UIContainer::FromJSON(const json& j) {
	UIWidget::FromJSON(j);
	_flow = j.contains("flow") ? DirectionMap.strToEnum.at(j["flow"]) : Direction::Vertical;
	_gap = j.value("gap", 0.0f);
}

void UIContainer::_Arrange() {
	const Rect& computed = _layout->GetComputedRect();
	const Sides& padding = _layout->GetPadding();
	Rect inner = {
		computed.x + padding.left,
		computed.y - padding.top,
		computed.width - padding.left - padding.right,
		computed.height - padding.top - padding.bottom
	};

	switch (_flow) {
		case Direction::Vertical: {
			float cursor = inner.y;
			for (auto child : _children) {
				Rect childAvailable = { inner.x, cursor, inner.width, inner.height - (inner.y - cursor) };
				child->Arrange(childAvailable);
				cursor -= child->GetLayout()->GetComputedRect().height + _gap;
			}
			break;
		}
		case Direction::Horizontal: {
			float cursor = inner.x;
			for (auto child : _children) {
				Rect childAvailable = { cursor, inner.y, inner.width - (cursor - inner.x), inner.height };
				child->Arrange(childAvailable);
				cursor += child->GetLayout()->GetComputedRect().width + _gap;
			}
			break;
		}
		case Direction::Depth: {
			for (auto child : _children) {
				child->Arrange(inner);
			}
			break;
		}
	}
}

void UIContainer::AddChild(const std::shared_ptr<UIWidget> child) {
	_children.push_back(child);
	MarkDirty();
}

void UIContainer::Draw(Renderer& renderer) {
	for (auto child : _children) {
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
	if (auto parent = _parent.lock()) {
		parent->RemoveChild(this);
	}
	_children.clear();
}

void UIContainer::DirtyChildren() {
	_dirty = true;
	for (auto child : _children) {
		if (auto container = std::dynamic_pointer_cast<UIContainer>(child)) {
			container->DirtyChildren();
		} else {
			child->MarkDirty();
		}
	}
}

glm::vec2 UIContainer::MeasureContent() {
	if (_children.empty()) {
		return glm::vec2(0.0f);
	}
	switch (_flow) {
		case Direction::Vertical: {
			float maxWidth = 0;
			float totalHeight = 0;
			for (auto& child : _children) {
				glm::vec2 childSize = child->MeasureContent();
				
				maxWidth = std::max(maxWidth, childSize.x);
				totalHeight += childSize.y;
			}
			totalHeight += _gap * (_children.size() - 1);
			return glm::vec2(maxWidth, totalHeight);
		}
		case Direction::Horizontal: {
			float totalWidth = 0;
			float maxHeight = 0;
			for (auto& child : _children) {
				glm::vec2 childSize = child->MeasureContent();
				
				totalWidth += childSize.x;
				maxHeight = std::max(maxHeight, childSize.y);
			}
			totalWidth += _gap * (_children.size() - 1);
			return glm::vec2(totalWidth, maxHeight);
		}
		case Direction::Depth: {
			float maxWidth = 0;
			float maxHeight = 0;
			for (auto& child : _children) {
				glm::vec2 childSize = child->MeasureContent();

				maxWidth = std::max(maxWidth, childSize.x);
				maxHeight = std::max(maxHeight, childSize.y);
			}
			return glm::vec2(maxWidth, maxHeight);
		}
	}
}
