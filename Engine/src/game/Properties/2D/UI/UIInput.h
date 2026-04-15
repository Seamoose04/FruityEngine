#pragma once

#include "game/Properties/2D/UI/UIClickable.h"
#include "game/Properties/2D/UI/UILabel.h"
#include "game/Properties/2D/UI/UIPanel.h"
#include "game/Properties/2D/UI/UIInterfaces.h"
#include "game/Properties/2D/UI/UIIcon.h"
#include "game/Properties/Input/InputHandler.h"
#include "util/Callback.h"

class UIInput : public UIClickable, IFocusable {
public:
	void FromJSON(const json& j) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;

	void OnFocus() override;
	void OnBlur() override;

	Callback<std::string> onChange, onCommit;

private:
	std::string _initialTextAlign;
	std::string _initialText;
	std::array<float, 4> _initialColor;
	std::array<float, 4> _initialTextColor;
	float _initialFontSize;
	std::string _initialFont;
	std::shared_ptr<UIPanel> _bg;
	std::shared_ptr<UILabel> _label;
	std::shared_ptr<InputHandler> _inputHandler;
	std::shared_ptr<UIIcon> _cursor;
};
