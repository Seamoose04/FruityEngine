#include "Transform.h"

void Transform::FromJSON(const json& data)
{
    if (data.contains("position")) {
        position = glm::vec3(data["position"][0], data["position"][1], data["position"][2]);
    }

    if (data.contains("rotation")) {
        rotation = glm::vec3(data["rotation"][0], data["rotation"][1], data["rotation"][2]);
    }

    if (data.contains("scale")) {
        scale = glm::vec3(data["scale"][0], data["scale"][1], data["scale"][2]);
    }
}

REGISTER_PROPERTY(Transform)