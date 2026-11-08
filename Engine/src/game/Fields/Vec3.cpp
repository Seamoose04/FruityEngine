#include "Vec3.h"

void Vec3Field::FromJSON(const json& data) {
	if (data.is_array()) {
		value = {
			data[0],
			data[1],
			data[2]
		};
	} else if (data.is_object()) {
		int idx = 0;
		for (auto& [k, v] : data.items()) {
			value[idx] = v;
			idx++;
		}
	}
}
