#pragma once

#include "game/Property.h"
#include "game/Properties/2D/UI/UITypes.h"
#include "game/PropertyRef.h"
#include "game/Properties/2D/UI/UILayout.h"
#include "game/Properties/Camera/Camera.h"
#include <memory>

class UIWidget : public Property {
public:
	void FromJSON(const json& j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void OnDestroy() override;
	virtual void HandleInput(const Window &window, float dt) override {};

	void Arrange(Rect availableRect);
	virtual void Draw(Renderer& renderer) = 0;
	UILayout* GetLayout();
	int GetZIndex() const;
	virtual glm::vec2 MeasureContent();
	void MarkDirty();
	void AddChild(std::shared_ptr<UIWidget> child);
	void DirtyChildren();

protected:
	virtual void _Arrange() = 0;
	float _ResolveAxis(Size size, float available, float measured = 0.0f);

	int _zIndex = 0;
	bool _dirty = true;
	PropertyRef<UILayout> _layout;
	std::weak_ptr<UIWidget> _parent;
	std::vector<std::shared_ptr<UIWidget>> _children;
	Camera* _camera = nullptr;
};
