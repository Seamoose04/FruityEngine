#include "Vec2.h"

void Vec2Field::FromJSON(const json& data) {
	if (data.is_array()) {
		value = {
			data[0],
			data[1]
		};
	} else if (data.is_object()) {
		int idx = 0;
		for (auto& [k, v] : data.items()) {
			value[idx] = v;
			idx++;
		}
	}
}
