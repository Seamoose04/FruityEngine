#pragma once

#include "graphics/PostProcessEffect.h"
#include "util/EnumMap.h"

class CombineEffect : public PostProcessEffect {
public:
    enum class Method {
        Add = 0,
        Multiply = 1
    };
    void Init(int width, int height) override;
    void Execute() override;
    void FromJSON(const json& data) override;

private:
    Shader _shader;
    Method _method;
    EnumMap<Method> _methodMap = {
        { "Add", Method::Add },
        { "Multiply", Method::Multiply }
    };
};
