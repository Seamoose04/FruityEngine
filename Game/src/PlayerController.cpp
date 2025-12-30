#pragma once
#include "game/Property.h"
#include <iostream>
#include "graphics/Renderer.h"

class PlayerController : public Property {
public:
	void FromJSON(const json& data, const std::string& scenePath) override {
		_startMessage = data["startMessage"];
	}
	void OnCreate() override {
		std::cout << "Player says: " << _startMessage << "\n";
	}
	void Update(float dt) override {
		
	}

private:
	std::string _startMessage;
};

REGISTER_PROPERTY(PlayerController)
