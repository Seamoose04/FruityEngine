#pragma once

#include "game/Property.h"
#include "UITypes.h"

class UILayout : public Property {
public:
	void FromJSON(const json &data) override;
	const Size& GetWidth() const;
	const Size& GetHeight() const;
	const Sides& GetMargin() const;

	void SetWidth(const Size& width);
	void SetHeight(const Size& height);
	void SetMargin(const Sides& margin);

	Align GetAlignSelf() const;
	const Rect& GetComputedRect() const;
	Rect& GetComputedRect();
	static Size Pixels(float v) { return { Size::Mode::Pixels, v }; }
	static Size Percent(float v) { return { Size::Mode::Pixels, v }; }
	static Size Auto() { return { Size::Mode::Pixels, 0.0f }; }

private:
	Size _width, _height;
	Sides _margin = { 0.0f, 0.0f, 0.0f, 0.0f };
	Align _alignSelf = Align::Center;
	Rect _computedRect;
};
