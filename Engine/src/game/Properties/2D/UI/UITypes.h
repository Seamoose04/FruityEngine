#pragma once

#include "util/EnumMap.h"
#include <json.hpp>

struct Size {
	enum class Mode {
		Pixels,
		Percent,
		Auto
	};
	inline static const EnumMap<Mode> _modeMap = {
		{ "Pixels", Mode::Pixels },
		{ "Percent", Mode::Percent },
		{ "Auto", Mode::Auto }
	};
	static Size FromJSON(const nlohmann::json& j) {
		auto mode = _modeMap.strToEnum.at(j["mode"].get<std::string>());
		float value = (mode != Mode::Auto) ? j["value"].get<float>() : 0.0f;
		return { mode, value };
	};
	Mode mode;
	float value;
};

struct Sides {
	float top, right, bottom, left;
	static Sides FromJSON(const nlohmann::json& j) {
		Sides sides;
		if (j.is_number()) {
			float v = j.get<float>();
			return { v, v, v, v };
		}
		return {
			j.value("top", 0.0f),
			j.value("right", 0.0f),
			j.value("bottom", 0.0f),
			j.value("left", 0.0f)
		};
	};
};

struct Rect {
	float x, y, width, height;
	static Rect FromJSON(const nlohmann::json& j) {
		return {
			j.value("x", 0.0f),
			j.value("y", 0.0f),
			j.value("width", 0.0f),
			j.value("height", 0.0f)
		};
	};
};

enum class Align {
	Start,
	Center,
	End
};

inline static const EnumMap<Align> AlignMap = {
	{ "Start", Align::Start },
	{ "Center", Align::Center },
	{ "End", Align::End }
};

enum class Direction {
	Vertical,
	Horizontal,
	Depth
};

inline static const EnumMap<Direction> DirectionMap = {
	{ "Vertical", Direction::Vertical },
	{ "Horizontal", Direction::Horizontal },
	{ "Depth", Direction::Depth }
};
