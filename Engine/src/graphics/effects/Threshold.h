#pragma once

#include "graphics/PostProcessEffect.h"

class ThresholdEffect : public PostProcessEffect {
public:
    void Init(int width, int height) override;
    void Execute() override;
    void FromJSON(const json& data) override;

private:
    float _threshold = 1.0f;
    float _knee = 0.0f;
    Shader _shader;
};