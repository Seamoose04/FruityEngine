#include "UICanvas.h"
#include "game/Property.h"
#include <algorithm>

void UICanvas::OnCreate(std::weak_ptr<Scene> scene) {
	_screenSize = scene.lock()->GetRenderer().GetSize();
}

void UICanvas::Update(float dt) {
	if (_dirty) {
		for (auto& widget : _widgets) {
			widget->Arrange({ -_screenSize.x / 2, -_screenSize.y / 2, _screenSize.x, _screenSize.y });
		}
		_dirty = false;
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
	MarkDirty();
}

void UICanvas::RegisterWidget(UIWidget* widget) {
	_widgets.push_back(widget);
	MarkDirty();
}

void UICanvas::MarkDirty() {
	_dirty = true;
}

REGISTER_PROPERTY(UICanvas)
