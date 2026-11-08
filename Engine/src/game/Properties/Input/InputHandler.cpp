#include "InputHandler.h"
#include "game/Property.h"
#include "game/Scene.h"

void InputHandler::FromJSON(const json& j) {
	_Insert(j.value("initialValue", ""));
}

void InputHandler::OnCreate(std::weak_ptr<Scene> scene) {
	_scene = scene;
}

void InputHandler::_MoveCursor(int amount) {
	_cursorPos = static_cast<unsigned int>(
		std::clamp(static_cast<int>(_cursorPos) + amount, 0, static_cast<int>(_value.size()))
	);
	onCursorMoved.Call(_cursorPos, _value);
}

bool InputHandler::_Accept(const char& c) const {
	return true;
}

void InputHandler::_Insert(const std::string& text) {
	std::string filtered = text;
	std::erase_if(filtered, [this](char c) {
		return !_Accept(c);
	});
	_value.insert(_cursorPos, filtered);
	_MoveCursor(filtered.size());
	onChange.Call(_value);
}

void InputHandler::_Backspace() {
	if (_value.empty() || _cursorPos <= 0) {
		return;
	}
	_value.erase(_cursorPos - 1, 1);
	_MoveCursor(-1);
	onChange.Call(_value);
}

void InputHandler::_Delete() {
	if (_value.empty() || _cursorPos >= _value.size()) {
		return;
	}
	_value.erase(_cursorPos, 1);
	onChange.Call(_value);
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
			if (codepoint <= 127) {
				_Insert(std::string(1, static_cast<char>(codepoint)));
			} else {
				// TODO: Support full Unicode codepoints.
				// The static_cast only works for ASCII (0-127).
				// Non-ASCII (Unicode) will require UTF-8, with multi-byte encoding.
			}
		});
	}
	if (window.IsKeyJustPressed(GLFW_KEY_BACKSPACE)) {
		_Backspace();
	}
	if (window.IsKeyJustPressed(GLFW_KEY_DELETE)) {
		_Delete();
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

void InputHandler::SetInput(const std::string& input) {
	_value = "";
	_cursorPos = 0;
	_Insert(input);
}

void InputHandler::_Submit() {
	_cursorPos = _value.length();
	onCommit.Call(_value);
}

void InputHandler::SetFocused(bool focus) {
	if (_focused && !focus) {
		_Submit();
	}
	_focused = focus;
}

REGISTER_PROPERTY(InputHandler)
