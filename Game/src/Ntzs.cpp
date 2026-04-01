#include "game/Property.h"
#include "game/PropertyRef.h"
#include "game/Properties/3D/Transform.h"

class Ntzs : public Property {
public:
	void FromJSON(const json& data) override {
		_min = data.value("min", 0.5f);
		_max = data.value("max", 1.5f);
		_speed = data.value("speed", 1.0f);
	}
    void OnCreate(std::weak_ptr<Scene> scene) override {
        _transform.From(_gameObject);
    }
	void Update(float dt) override {
		_time += dt * _speed;
		float size = -std::abs(2 * std::sin(_time)+1) + 3;
		_transform->SetLocalScale(glm::vec3((size / 3) * (_max - _min) + _min));
	}

private:
    PropertyRef<Transform> _transform;
	float _min = 1.0f;
	float _max = 1.0f;
	float _speed = 1.0;
	float _time = 0.0f;
};

REGISTER_PROPERTY(Ntzs)
