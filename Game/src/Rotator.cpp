#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Properties/Transform.h"

class Rotator : public Property {
public:
	void FromJSON(const json& data) override {
		_rotation = glm::vec3(
			data.contains("xRot") ? (float)data["xRot"] : 0.0f,
			data.contains("yRot") ? (float)data["yRot"] : 0.0f,
			data.contains("zRot") ? (float)data["zRot"] : 0.0f
		);
	}
    void OnCreate(std::weak_ptr<Scene> scene) override {
        _transform.From(_parent);
    }
	void Update(float dt) override {
		_transform->Rotate(_rotation * dt);
	}

private:
    PropertyRef<Transform> _transform;
	glm::vec3 _rotation;
};

REGISTER_PROPERTY(Rotator)
