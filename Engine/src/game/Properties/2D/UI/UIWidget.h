#pragma once

#include "game/Property.h"
#include "game/Properties/2D/UITypes.h"
#include "game/PropertyRef.h"
#include "game/Properties/2D/UILayout.h"
#include <memory>

class UIWidget : public Property {
public:
	void FromJSON(const json& j) override;
	void AddChild(std::shared_ptr<UIWidget> child);
	void OnCreate(std::weak_ptr<Scene> scene) override;

	virtual void Arrange(Rect availableRect) = 0;
	virtual void Draw(Renderer& renderer) = 0;
	UILayout* GetLayout();
	int GetZIndex() const;

protected:
	PropertyRef<UILayout> _layout;
	std::vector<std::shared_ptr<UIWidget>> _children;
	int _zIndex = 0;
};
