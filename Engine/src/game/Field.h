#pragma once

#include <json.hpp>

using json = nlohmann::json;
class Field {
public:
	virtual void FromJSON(const json& data);
};
