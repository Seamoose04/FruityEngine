#pragma once

#include "graphics/PostProcessEffect.h"

class BlurEffect : public PostProcessEffect {
public:
    void Init(int width, int height) override;
    void Execute() override;
    void FromJSON(const json& data) override;
    Framebuffer* GetOutput() override;

private:
    int _passes;
    float _size = 1.0f;
    Framebuffer _pingPong[2];
    Shader _shader;
};