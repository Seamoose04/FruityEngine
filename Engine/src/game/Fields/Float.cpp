#include "Float.h"

void FloatField::FromJSON(const json& data) {
	value = data;
}
