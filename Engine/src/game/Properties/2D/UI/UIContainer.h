#pragma once
#include "UIWidget.h"

class UIContainer : public UIWidget {
public:
	void FromJSON(const json &j) override;
	void Draw(Renderer &renderer) override;
	void OnDestroy() override;
	std::vector<UIWidget*> ActiveChildren() const;

	const Sides& GetPadding() const;
	void SetPadding(const Sides& padding);

	virtual void AddChild(std::shared_ptr<UIWidget> child);
	virtual void RemoveChild(UIWidget* child);
	virtual glm::vec2 MeasureContent() override = 0;

protected:
	virtual void _Arrange() override;
	std::vector<std::shared_ptr<UIWidget>> _children;
	Sides _padding;
};
