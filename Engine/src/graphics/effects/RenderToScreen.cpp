#include "RenderToScreen.h"

#include "util/ShaderCache.h"
#include <glad/glad.h>
#include <iostream>

static const std::string ENGINE_SHADER_PATH = "../../Engine/shaders/fx/";

void RenderToScreenEffect::Init(int width, int height) {
    _width = width;
    _height = height;
    _shader = ShaderCache::Instance().Get(ENGINE_SHADER_PATH + "screen.vert", ENGINE_SHADER_PATH + "screen.frag");
}

void RenderToScreenEffect::Execute() {
    auto* input = GetInput("input");
    if (!input) {
        std::cerr << "[RenderToScreen] No input connected" << std::endl;
        return;
    }

    input->Execute();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    _shader->Use();
    _shader->SetInt("u_Input", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, input->GetOutput()->GetColorTexture());

    DrawQuad();

    glEnable(GL_DEPTH_TEST);
}

void RenderToScreenEffect::Resize(int width, int height) {
    _width = width;
    _height = height;
}

Framebuffer* RenderToScreenEffect::GetOutput() {
    return nullptr;
}
