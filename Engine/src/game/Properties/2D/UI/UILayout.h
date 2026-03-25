#pragma once

#include "game/Property.h"
#include "UITypes.h"

class UILayout : public Property {
public:
	void FromJSON(const json &data) override;
	const Size& GetWidth() const;
	const Size& GetHeight() const;
	const Sides& GetMargin() const;
	const Sides& GetPadding() const;
	Align GetAlignSelf() const;
	Rect& GetComputedRect();
	static Size Pixels(float v) { return { Size::Mode::Pixels, v }; }
	static Size Percent(float v) { return { Size::Mode::Pixels, v }; }
	static Size Auto() { return { Size::Mode::Pixels, 0.0f }; }

private:
	Size _width, _height;
	Sides _margin = { 0.0f, 0.0f, 0.0f, 0.0f };
	Sides _padding = { 0.0f, 0.0f, 0.0f, 0.0f };
	Align _alignSelf = Align::Center;
	Rect _computedRect;
};
