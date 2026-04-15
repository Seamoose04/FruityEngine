#include "Blur.h"

#include <glad/glad.h>
#include <iostream>
#include "util/ShaderCache.h"

static const std::string ENGINE_SHADER_PATH = "../../Engine/shaders/fx/";

void BlurEffect::FromJSON(const json& data) {
    _size = data.value("size", 1.0f);
    _passes = data.value("passes", 0);
}

void BlurEffect::Init(int width, int height) {
    _pingPong[0] = Framebuffer(width, height, GL_RGBA16F);
    _pingPong[1] = Framebuffer(width, height, GL_RGBA16F);
	_shader = ShaderCache::Instance().Get(ENGINE_SHADER_PATH + "blur.vert", ENGINE_SHADER_PATH + "blur.frag");
}

void BlurEffect::Execute() {
    auto* input = GetInput("input");
    if (!input) {
        std::cerr << "[RenderToScreen] No input connected" << std::endl;
        return;
    }
    input->Execute();

    glDisable(GL_DEPTH_TEST);    
    _shader->Use();
    _shader->SetInt("u_Input", 0);
    _shader->SetFloat("u_Size", _size);

    unsigned int srcTexture = input->GetOutput()->GetColorTexture();
    for (int i = 0; i < _passes * 2; i++) {
        int dest = i % 2;
        
        _pingPong[dest].Bind();

        glClear(GL_COLOR_BUFFER_BIT);
        _shader->SetBool("u_Horizontal", dest == 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, srcTexture);

        DrawQuad();

        _pingPong[dest].Unbind();

        srcTexture = _pingPong[dest].GetColorTexture();
    }

    glEnable(GL_DEPTH_TEST);
}

Framebuffer* BlurEffect::GetOutput() {
    return &_pingPong[1];
}

REGISTER_EFFECT(BlurEffect)
