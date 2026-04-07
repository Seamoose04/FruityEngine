#pragma once
#include "UIWidget.h"

class UIContainer : public UIWidget {
public:
	void FromJSON(const json &j) override;
	void Draw(Renderer &renderer) override;
	void AddChild(std::shared_ptr<UIWidget> child);
	void OnDestroy() override;
	void RemoveChild(UIWidget* child);
	void DirtyChildren();
	glm::vec2 MeasureContent() override;

protected:
	void _Arrange() override;
	Direction _flow;
	float _gap;
	std::vector<std::shared_ptr<UIWidget>> _children;
};
