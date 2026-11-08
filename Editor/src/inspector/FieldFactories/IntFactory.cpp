#include "EditorScene.h"
#include "game/Fields/Int.h"
#include "inspector/FieldFactoryRegistry.h"
#include <memory>

std::unique_ptr<GameObject> IntFactory(const std::shared_ptr<Field> &field) {
	std::shared_ptr<IntField> intField = std::static_pointer_cast<IntField>(field);

	json j = {

	};

	std::unique_ptr<GameObject> go = std::make_unique<GameObject>();
	go->FromJSON(j);

	return go;
}

REGISTER_FIELD_FACTORY(IntField, IntFactory)
