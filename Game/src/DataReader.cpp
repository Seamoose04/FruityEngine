#include "game/Property.h"
#include <iostream>

class DataReader : public Property {
public:
	void FromJSON(const json &data) override {
		_data = data;
	}
	void OnCreate(std::weak_ptr<Scene> scene) override {
		std::cout << _data << std::endl;
	}

private:
	json _data;
};

REGISTER_PROPERTY(DataReader)
