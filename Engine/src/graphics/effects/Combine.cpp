#include "Combine.h"

#include <glad/glad.h>

static const std::string ENGINE_SHADER_PATH = "../../Engine/shaders/";

void CombineEffect::FromJSON(const json& data) {
    _method = _methodMap.strToEnum[data.value("method", "")];
}

void CombineEffect::Init(int width, int height) {
    _output = Framebuffer(width, height, GL_RGBA16F);
    _shader.Load(ENGINE_SHADER_PATH + "combine.vert", ENGINE_SHADER_PATH + "combine.frag");
}

void CombineEffect::Execute() {
    auto* inputA = GetInput("inputA");
    auto* inputB = GetInput("inputB");
    if (!inputA || !inputB) {
        return;
    }
    inputA->Execute();
    inputB->Execute();

    _output.Bind();
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    _shader.Use();
    _shader.SetInt("u_InputA", 0);
    _shader.SetInt("u_InputB", 1);
    _shader.SetInt("u_Method", static_cast<int>(_method));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputA->GetOutput()->GetColorTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, inputB->GetOutput()->GetColorTexture());

    DrawQuad();

    _output.Unbind();
    glEnable(GL_DEPTH_TEST);
}

REGISTER_EFFECT(CombineEffect)
