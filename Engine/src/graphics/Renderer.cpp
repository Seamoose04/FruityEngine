#include "Renderer.h"
#include <glad/glad.h>
#include <iostream>
#include "game/Properties/Camera.h"

Renderer::Renderer(int width, int height)
    : _width(width), _height(height), _clearColor(0.1f, 0.1f, 0.15f, 1.0f)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    std::cout << "Renderer initialized (" << width << "x" << height << ")\n";
}

Renderer::~Renderer() {}

void Renderer::BeginFrame() {
    glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {
}

void Renderer::ClearColor(float r, float g, float b, float a) {
    _clearColor = glm::vec4(r, g, b, a);
}

void Renderer::Resize(int w, int h) {
	_width = w;
	_height = h;
}

void Renderer::DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& modelMatrix, const Camera& camera) {
    shader.Use();
    glm::mat4 mvp = camera.GetProjection() * camera.GetView() * modelMatrix;
    shader.SetMat4("u_MVP", mvp);

    mesh.Bind();
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}
