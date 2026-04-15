#include "UIButton.h"

void UIButton::FromJSON(const json& j) {
	UIClickable::FromJSON(j);
	if (j.contains("onClick")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onClick"]);
		onClick.Subscribe([call]() {
			call.Invoke();
		});
	}
	if (j.contains("onRelease")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onRelease"]);
		onRelease.Subscribe([call]() {
			call.Invoke();
		});
	}
	if (j.contains("onEnter")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onEnter"]);
		onEnter.Subscribe([call]() {
			call.Invoke();
		});
	}
	if (j.contains("onExit")) {
		ActionRegistry::ActionCall call;
		call.FromJSON(j["onExit"]);
		onExit.Subscribe([call]() {
			call.Invoke();
		});
	}
}

REGISTER_PROPERTY(UIButton)
