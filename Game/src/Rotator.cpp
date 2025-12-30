#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Properties/Transform.h"
#include <cmath>

class Rotator : public Property {
public:
	void FromJSON(const json& data, std::weak_ptr<Scene> scene) override {
		_xRot = data.contains("xRot") ? (float)data["xRot"] : 0.0f;
		_yRot = data.contains("yRot") ? (float)data["yRot"] : 0.0f;
		_zRot = data.contains("zRot") ? (float)data["zRot"] : 0.0f;
	}
    void OnCreate() override {
        _transform.From(_parent);
    }
	void Update(float dt) override {
		glm::vec3 rot = _transform->rotation;
		_transform->rotation.x = std::fmod(rot.x + _xRot * dt, 360.0f);
		_transform->rotation.y = std::fmod(rot.y + _yRot * dt, 360.0f);
		_transform->rotation.z = std::fmod(rot.z + _zRot * dt, 360.0f);
	}

private:
    PropertyRef<Transform> _transform;
    float _xRot;
    float _yRot;
    float _zRot;
};

REGISTER_PROPERTY(Rotator)
