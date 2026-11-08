#pragma once

#include "game/Properties/Input/InputHandler.h"

class IntInputHandler : public InputHandler {
private:
	bool _Accept(const char &c) const override;
};
