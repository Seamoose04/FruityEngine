#include "UIIntInput.h"

#include "game/Properties/Input/InputHandlers/IntInputHandler.h"

void UIIntInput::_InitInputHandler() {
	json inputHandlerChunk = {
		{ "IntInputHandler", {
			{ "initialValue", _initialText }
		}}
	};
	_inputHandler = _gameObject.lock()->AddProperty<IntInputHandler>(inputHandlerChunk);
}

REGISTER_PROPERTY(IntInputHandler)
