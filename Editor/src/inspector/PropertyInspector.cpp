#include "PropertyInspector.h"

void PropertyInspector::FromJSON(const json &data) {
	UIWidget::FromJSON(data);
	if (data.contains("property")) {
		_property = data["property"];
	}
}
void PropertyInspector::OnCreate(std::weak_ptr<Scene> scene) {
	UIWidget::OnCreate(scene);
}

REGISTER_PROPERTY(PropertyInspector)
