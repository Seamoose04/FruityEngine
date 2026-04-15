#pragma once

#include "game/Property.h"
#include "util/Callback.h"

class InputHandler : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void HandleInput(const Window &window, float dt) override;

	void SetValue(std::string value);
	void SetFocused(bool focus);

	Callback<std::string> onChange, onCommit;
	Callback<int, std::string> onCursorMoved;
	Callback<> onBlurRequested;

private:
	void _MoveCursor(int amount);
	bool _focused;
	int _cursorPos;
	std::optional<size_t> _callbackId;
	std::string _value = "";
	std::weak_ptr<Scene> _scene;
};
