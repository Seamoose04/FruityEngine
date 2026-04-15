#pragma once

class IFocusable {
public:
	virtual void OnFocus() = 0;
	virtual void OnBlur() = 0;
	virtual ~IFocusable() = default;
};
