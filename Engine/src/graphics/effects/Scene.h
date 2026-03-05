#pragma once

#include "graphics/PostProcessEffect.h"

class SceneEffect : public PostProcessEffect {
public:
    void Init(int width, int height) override {}
    void Execute() override {}

    Framebuffer* GetOutput() override;
    void SetHDRBuffer(Framebuffer* buffer);

private:
    Framebuffer* _hdrBuffer = nullptr;
};