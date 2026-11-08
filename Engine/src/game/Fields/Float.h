#pragma once

#include "game/Field.h"

class FloatField : public Field {
public:
	void FromJSON(const json &data) override;
	
	float value;
};
