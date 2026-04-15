#include "UILayout.h"

void UILayout::FromJSON(const json& j) {
	_width = Size::FromJSON(j["width"]);
	_height = Size::FromJSON(j["height"]);
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

void UILayout::SetWidth(const Size& width) {
	_width = width;
}

void UILayout::SetHeight(const Size& height) {
	_height = height;
}

void UILayout::SetMargin(const Sides& margin) {
	_margin = margin;
}

const Rect& UILayout::GetComputedRect() const {
	return _computedRect;
}

Rect& UILayout::GetComputedRect() {
	return _computedRect;
}

REGISTER_PROPERTY(UILayout)
