#pragma once

#include "graphics/PostProcessEffect.h"

class DownsampleEffect : public PostProcessEffect {
public:
	void Init(int width, int height) override;
	void Execute() override;
	void FromJSON(const json& data) override;
	void Resize(int width, int height) override;

private:
	Shader _shader;
	float _scale = 0.5f;
};
