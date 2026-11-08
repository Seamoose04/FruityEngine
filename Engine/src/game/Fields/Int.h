#pragma once

#include "game/Field.h"

class IntField : public Field {
public:
	void FromJSON(const json &data) override;
	
	int value;
};
