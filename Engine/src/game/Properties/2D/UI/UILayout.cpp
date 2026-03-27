#include "UILayout.h"

void UILayout::FromJSON(const json& j) {
	_width = Size::FromJSON(j["width"]);
	_height = Size::FromJSON(j["height"]);
	if (j.contains("padding")) {
		_padding = Sides::FromJSON(j["padding"]);
	}
	if (j.contains("margin")) {
		_margin = Sides::FromJSON(j["margin"]);
	}
	if (j.contains("alignSelf")) {
		_alignSelf = AlignMap.strToEnum.at(j["alignSelf"]);
	}
}

const Size& UILayout::GetWidth() const {
	return _width;
}

const Size& UILayout::GetHeight() const {
	return _height;
}

const Sides& UILayout::GetMargin() const {
	return _margin;
}

const Sides& UILayout::GetPadding() const {
	return _padding;
}

const Rect& UILayout::GetComputedRect() const {
	return _computedRect;
}

Rect& UILayout::GetComputedRect() {
	return _computedRect;
}

REGISTER_PROPERTY(UILayout)
