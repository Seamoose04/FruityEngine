#include "Upsample.h"
#include <glad/glad.h>

static const std::string ENGINE_SHADER_PATH = "../../Engine/shaders/fx/";

void UpsampleEffect::Init(int width, int height) {
    _output = Framebuffer(width, height, GL_RGBA16F);
    _shader.Load(ENGINE_SHADER_PATH + "upsample.vert", ENGINE_SHADER_PATH + "upsample.frag");
}

void UpsampleEffect::Execute() {
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

    _output.Unbind();
    glEnable(GL_DEPTH_TEST);
}

REGISTER_EFFECT(UpsampleEffect)
