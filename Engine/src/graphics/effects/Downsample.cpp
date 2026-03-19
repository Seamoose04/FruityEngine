#include "Downsample.h"

#include <glad/glad.h>

static const std::string ENGINE_SHADER_PATH = "../../Engine/shaders/fx/";

void DownsampleEffect::FromJSON(const json& data) {
	_scale = data.value("scale", 0.5);
}

void DownsampleEffect::Init(int width, int height) {
	_output = Framebuffer((int)(width * _scale), (int)(height * _scale), GL_RGBA16F);
	_shader.Load(ENGINE_SHADER_PATH + "downsample.vert", ENGINE_SHADER_PATH + "downsample.frag");
}

void DownsampleEffect::Execute() {
	auto* input = GetInput("input");
	if (!input) {
		return;
	}
	input->Execute();

	_output.Bind();
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	_shader.Use();
	_shader.SetInt("u_Input", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input->GetOutput()->GetColorTexture());

	DrawQuad();

	_output.Bind();
	glEnable(GL_DEPTH_TEST);
}

void DownsampleEffect::Resize(int width, int height) {
	_output.Resize((int)(width * _scale), (int)(height * _scale));
}

REGISTER_EFFECT(DownsampleEffect)
