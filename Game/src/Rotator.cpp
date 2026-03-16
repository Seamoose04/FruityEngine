#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Properties/Transform.h"
#include <cmath>

class Rotator : public Property {
public:
	void FromJSON(const json& data) override {
		_xRot = data.contains("xRot") ? (float)data["xRot"] : 0.0f;
		_yRot = data.contains("yRot") ? (float)data["yRot"] : 0.0f;
		_zRot = data.contains("zRot") ? (float)data["zRot"] : 0.0f;
	}
    void OnCreate(std::weak_ptr<Scene> scene) override {
        _transform.From(_parent);
    }
	void Update(float dt) override {
		glm::vec3 rot = _transform->GetLocalRotation();
		_transform->SetLocalRotation(glm::vec3(
			std::fmod(rot.x + _xRot * dt, 360.0f),
			std::fmod(rot.y + _yRot * dt, 360.0f),
			std::fmod(rot.z + _zRot * dt, 360.0f)
		));
	}

private:
    PropertyRef<Transform> _transform;
    float _xRot;
    float _yRot;
    float _zRot;
};

REGISTER_PROPERTY(Rotator)
