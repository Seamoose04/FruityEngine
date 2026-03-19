#pragma once
#include "graphics/PostProcessEffect.h"

class UpsampleEffect : public PostProcessEffect {
public:
    void Init(int width, int height) override;
    void Execute() override;

private:
    Shader _shader;
};
