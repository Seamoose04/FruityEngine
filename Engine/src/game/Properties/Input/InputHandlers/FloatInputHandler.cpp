#include "FloatInputHandler.h"

#include <cctype>

bool FloatInputHandler::_Accept(const char& c) const {
	if (c == '.') {
		return !_decimalPos.has_value();
	}
	return std::isdigit(static_cast<unsigned char>(c));
}

void FloatInputHandler::_Insert(const std::string& text) {
	std::string filtered = "";
	unsigned int tempCursorPos = 0;

	for (const char& c : text) {
		if (!_Accept(c)) {
			continue;
		}
		if (c == '.') {
			_decimalPos = _cursorPos + tempCursorPos;
		}
		tempCursorPos++;
	}

	_value.insert(_cursorPos, filtered);
	_MoveCursor(filtered.size());
	onChange.Call(_value);
}

void FloatInputHandler::_Backspace() {
	if (_cursorPos > 0 && _value[_cursorPos - 1] == '.') {
		_decimalPos.reset();
	}
	InputHandler::_Backspace();
}

void FloatInputHandler::_Delete() {
	if (_cursorPos < _value.length() && _value[_cursorPos] == '.') {
		_decimalPos.reset();
	}
	InputHandler::_Delete();
}

void FloatInputHandler::_Submit() {
	if (_value.empty()) {
		_Insert("0");
	}
	if (!_decimalPos.has_value()) {
		_cursorPos = _value.length();
		_Insert(".0");
	}
	InputHandler::_Submit();
}

REGISTER_PROPERTY(FloatInputHandler)
