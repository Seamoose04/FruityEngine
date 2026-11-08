#include "Int.h"

void IntField::FromJSON(const json& data) {
	value = data;
}
