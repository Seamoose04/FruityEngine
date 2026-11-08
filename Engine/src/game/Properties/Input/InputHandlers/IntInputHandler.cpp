#include "IntInputHandler.h"

#include <cctype>

bool IntInputHandler::_Accept(const char& c) const {
	return std::isdigit(static_cast<unsigned char>(c));
}

REGISTER_PROPERTY(IntInputHandler)
