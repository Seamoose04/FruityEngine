#pragma once

#include "graphics/PostProcessEffect.h"
#include "graphics/Shader.h"

class RenderToScreenEffect : public PostProcessEffect {
public:
    void Init(int width, int height) override;
    void Execute() override;
    void Resize(int width, int height) override;

    Framebuffer* GetOutput() override;

private:
    int _width = 0;
    int _height = 0;
    Shader _shader;
};