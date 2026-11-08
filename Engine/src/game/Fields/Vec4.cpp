#include "Vec4.h"

void Vec4Field::FromJSON(const json& data) {
	if (data.is_array()) {
		value = {
			data[0],
			data[1],
			data[2],
			data[3]
		};
	} else if (data.is_object()) {
		int idx = 0;
		for (auto& [k, v] : data.items()) {
			value[idx] = v;
			idx++;
		}
	}
}
