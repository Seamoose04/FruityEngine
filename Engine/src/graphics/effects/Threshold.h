#pragma once

#include "graphics/PostProcessEffect.h"

class ThresholdEffect : public PostProcessEffect {
public:
    void Init(int width, int height) override;
    void Execute() override;
    void FromJSON(const json& data) override;

private:
    int _width = 0;
    int _height = 0;
    float _threshold = 1.0f;
    Shader _shader;
};