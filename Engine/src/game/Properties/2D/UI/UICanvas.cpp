#include "UICanvas.h"
#include "game/Property.h"
#include <algorithm>

void UICanvas::FromJSON(const json& data) {
	_fullscreenX = !data.contains("width");
	_fullscreenY = !data.contains("height");
	_size = glm::vec2(data.value("width", 0.0f), data.value("height", 0.0f));
	_anchor = data.contains("anchor") ? glm::vec2(data["anchor"][0], data["anchor"][1]) : glm::vec2(0.0f);
	_pivot = data.contains("pivot") ? glm::vec2(data["pivot"][0], data["pivot"][1]) : glm::vec2(0.0f);
}

void UICanvas::OnCreate(std::weak_ptr<Scene> scene) {
	glm::vec2 size = scene.lock()->GetRenderer().GetSize();
	OnResize(size.x, size.y);
}

void UICanvas::Update(float dt) {
	glm::vec2 anchorPx = glm::vec2(_anchor.x * _screenSize.x / 2, _anchor.y * _screenSize.y / 2);
	glm::vec2 pivotPx = glm::vec2(_pivot.x * _size.x / 2, _pivot.y * _size.y / 2);
	glm::vec2 canvasCenter = anchorPx - pivotPx;
	glm::vec2 topLeft = glm::vec2(canvasCenter.x - _size.x / 2, canvasCenter.y + _size.y / 2);
	for (auto& widget : _widgets) {
		widget->Arrange({ topLeft.x, topLeft.y, _size.x, _size.y });
	}
}

void UICanvas::Render(Renderer& renderer) {
	std::sort(_widgets.begin(), _widgets.end(), [](UIWidget* a, UIWidget* b) {
		return a->GetZIndex() < b->GetZIndex();
	});
	for (auto* widget : _widgets) {
		widget->Draw(renderer);
	}
}

void UICanvas::OnResize(int width, int height) {
	_screenSize = glm::vec2(width, height);
	if (_fullscreenX) {
		_size.x = _screenSize.x;
	}
	if (_fullscreenY) {
		_size.y = _screenSize.y;
	}
	for (auto* widget : _widgets) {
		widget->DirtyChildren();
	}
}

void UICanvas::RegisterWidget(UIWidget* widget) {
	_widgets.push_back(widget);
}

void UICanvas::UnregisterWidget(UIWidget* widget) {
	_widgets.erase(
		std::remove_if(_widgets.begin(), _widgets.end(),
			[widget](UIWidget* child) {
				return child == widget;
			}),
		_widgets.end()
	);
}

REGISTER_PROPERTY(UICanvas)
