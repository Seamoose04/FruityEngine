#pragma once

#include "game/Property.h"
#include "util/Callback.h"

class InputHandler : public Property {
public:
	void FromJSON(const json &data) override;
	void OnCreate(std::weak_ptr<Scene> scene) override;
	void HandleInput(const Window &window, float dt) override;

	void SetFocused(bool focus);
	void SetInput(const std::string& input);

	Callback<std::string> onChange, onCommit;
	Callback<int, std::string> onCursorMoved;
	Callback<> onBlurRequested;

protected:
	virtual bool _Accept(const char& c) const;
	virtual void _Insert(const std::string& text);
	virtual void _Backspace();
	virtual void _Delete();
	virtual void _Submit();
	void _MoveCursor(int amount);
	unsigned int _cursorPos = 0;
	std::string _value = "";

private:
	bool _focused;
	std::optional<size_t> _callbackId;
	std::weak_ptr<Scene> _scene;
};
