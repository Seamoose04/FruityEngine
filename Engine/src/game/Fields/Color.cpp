#include "Color.h"

#include <glm/gtc/type_ptr.hpp>
#include <string>

void ColorField::FromJSON(const json& data) {
	if (data.is_string()) {
		std::string hex = data;
		hex = hex.substr(hex[0] == '#' ? 1 : 0);

		auto fromHex = [](const std::string& h) {
			return std::stoul(h, nullptr, 16) / 255.0f;
		};
		value = {
			fromHex(hex.substr(0, 2)),
			fromHex(hex.substr(2, 2)),
			fromHex(hex.substr(4, 2)),
			hex.size() == 8 ? fromHex(hex.substr(6, 2)) : 1.0f
		};
	} else if (data.is_array()) {
		value = {
			data[0].get<float>(),
			data[1].get<float>(),
			data[2].get<float>(),
			data.size() == 4 ? data[3].get<float>() : 1.0f
		};
	}
}
