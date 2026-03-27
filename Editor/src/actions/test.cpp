#include "game/ActionRegistry.h"
#include <iostream>

REGISTER_ACTION("test", "message", []() {
	std::cout << "[Test/Message] Hello World!" << std::endl;
});
