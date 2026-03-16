#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include "game/Properties/Camera.h"

Renderer::Renderer(int width, int height)
    : _width(width), _height(height),
    _clearColor(0.0f, 0.0f, 0.0f, 1.0f),
    _hdrBuffer(width, height, GL_RGBA16F)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    PostProcessEffect::InitQuad();

    std::cout << "Renderer initialized (" << width << "x" << height << ")\n";
}

void Renderer::BeginFrame() {
    _hdrBuffer.Bind();
    glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {
    _hdrBuffer.Unbind();

    if (_postProcessOutput) {
        _postProcessOutput->Execute();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, _width, _height);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::DrawMesh(const Mesh& mesh, std::shared_ptr<Shader> shader, const glm::mat4& modelMatrix, const Camera& camera) {
    glm::mat4 mvp = camera.GetProjection() * camera.GetView() * modelMatrix;
    shader->SetMat4("u_MVP", mvp);

    mesh.Bind();
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::SetPostProcessGraph(SceneEffect* sceneNode, RenderToScreenEffect* outputNode) {
    sceneNode->SetHDRBuffer(&_hdrBuffer);

    _postProcessOutput = outputNode;
    std::cout << "Post-process graph registered" << std::endl;
}

void Renderer::ClearPostProcessGraph() {
    _postProcessOutput = nullptr;
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    _clearColor = glm::vec4(r, g, b, a);
}

void Renderer::Resize(int w, int h) {
	_width = w;
	_height = h;
    _hdrBuffer.Resize(w, h);

    if (_postProcessOutput) {
        _postProcessOutput->Resize(w, h);
    }
}

int Renderer::GetWidth() const {
    return _width;
}

int Renderer::GetHeight() const {
    return _height;
}
