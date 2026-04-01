#include "game/ActionRegistry.h"
#include <iostream>

REGISTER_ACTION("test", "message", [](const json& j) {
	std::cout << "[Test/Message] Hello World!" << std::endl;
});
