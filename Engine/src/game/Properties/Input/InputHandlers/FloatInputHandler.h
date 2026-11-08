#pragma once

#include "game/Properties/Input/InputHandler.h"

#include <optional>

class FloatInputHandler : public InputHandler {
private:
	bool _Accept(const char &c) const override;
	void _Insert(const std::string &text) override;
	void _Backspace() override;
	void _Delete() override;
	void _Submit() override;
	std::optional<unsigned int> _decimalPos;
};
