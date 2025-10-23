#pragma once
#include "util/Registry.h"
#include "game/Property.h"
#include <glm/glm.hpp>

class Transform : public Property {
public:
    static std::string TypeName() { return "Transform"; }

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    void FromJSON(const json& data) override;
};