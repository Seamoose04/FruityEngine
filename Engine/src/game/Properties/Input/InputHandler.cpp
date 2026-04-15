#include "InputHandler.h"
#include "game/Property.h"
#include "game/Scene.h"

void InputHandler::FromJSON(const json& j) {
	_value = j.value("initialValue", "");
	_cursorPos = _value.size();
}

void InputHandler::OnCreate(std::weak_ptr<Scene> scene) {
	_scene = scene;
}

void InputHandler::_MoveCursor(int amount) {
	_cursorPos = std::clamp(_cursorPos + amount, 0, (int)_value.size());
	onCursorMoved.Call(_cursorPos, _value);
}

void InputHandler::HandleInput(const Window& window, float dt) {
	if (!_focused) {
		if (_callbackId.has_value()) {
			window.onChar.Unsubscribe(_callbackId.value());
			_callbackId = std::nullopt;
		}
		return;
	}
	if (!_callbackId.has_value()) {
		_callbackId = window.onChar.Subscribe([this](unsigned int codepoint) {
			SetValue(_value.insert(_cursorPos, 1, static_cast<char>(codepoint)));
			_MoveCursor(1);
		});
	}
	if (!_value.empty() && window.IsKeyJustPressed(GLFW_KEY_BACKSPACE) && _cursorPos > 0) {
		SetValue(_value.erase(_cursorPos - 1, 1));
		_MoveCursor(-1);
	}
	if (!_value.empty() && window.IsKeyJustPressed(GLFW_KEY_DELETE) && _cursorPos < _value.size()) {
		SetValue(_value.erase(_cursorPos, 1));
	}
	if (window.IsKeyJustPressed(GLFW_KEY_LEFT)) {
		_MoveCursor(-1);
	}
	if (window.IsKeyJustPressed(GLFW_KEY_RIGHT)) {
		_MoveCursor(1);
	}
	if (window.IsKeyJustPressed(GLFW_KEY_ENTER) || window.IsKeyJustPressed(GLFW_KEY_KP_ENTER)) {
		onBlurRequested.Call();
	}
	if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
		onBlurRequested.Call();
	}
}

void InputHandler::SetFocused(bool focus) {
	if (_focused && !focus) {
		onCommit.Call(_value);
	}
	_focused = focus;
}

void InputHandler::SetValue(std::string value) {
	_value = value;
	onChange.Call(_value);
}

REGISTER_PROPERTY(InputHandler)
