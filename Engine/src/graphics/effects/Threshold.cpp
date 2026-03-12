#include "Threshold.h"

#include <glad/glad.h>

static const std::string ENGINE_SHADER_PATH = "../../Engine/shaders/";

void ThresholdEffect::FromJSON(const json& data) {
    _threshold = data.value("threshold", 1.0f);
    _knee = data.value("knee", 0.0f);
}

void ThresholdEffect::Init(int width, int height) {
    _output = Framebuffer(width, height, GL_RGBA16F);
    _shader.Load(ENGINE_SHADER_PATH + "threshold.vert", ENGINE_SHADER_PATH + "threshold.frag");
}

void ThresholdEffect::Execute() {
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
    _shader.SetFloat("u_Threshold", _threshold);
    _shader.SetFloat("u_Knee", _knee);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, input->GetOutput()->GetColorTexture());

    DrawQuad();

    _output.Unbind();
    glEnable(GL_DEPTH_TEST);
}

REGISTER_EFFECT(ThresholdEffect)
