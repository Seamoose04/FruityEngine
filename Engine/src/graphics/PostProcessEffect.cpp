#include "PostProcessEffect.h"

#include <glad/glad.h>

unsigned int PostProcessEffect::_quadVAO = 0;
unsigned int PostProcessEffect::_quadVBO = 0;
bool PostProcessEffect::_quadInitialized = false;

static const float QUAD_VERTS[] = {
    // xy        // uv
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
};

void PostProcessEffect::InitQuad() {
    if (_quadInitialized) {
        return;
    }

    glGenVertexArrays(1, &_quadVAO);
    glGenBuffers(1, &_quadVBO);

    glBindVertexArray(_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTS), QUAD_VERTS, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

    glBindVertexArray(0);
    _quadInitialized = true;
}

void PostProcessEffect::DrawQuad() {
    glBindVertexArray(_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessEffect::SetInput(const std::string& slot, PostProcessEffect* effect) {
    _inputs[slot] = effect;
}

PostProcessEffect* PostProcessEffect::GetInput(const std::string& slot) const {
    auto it = _inputs.find(slot);
    if (it != _inputs.end()) {
        return it->second;
    }
    return nullptr;
}

Framebuffer* PostProcessEffect::GetOutput() {
    return &_output;
}

void PostProcessEffect::Resize(int width, int height) {
    _output.Resize(width, height);
}