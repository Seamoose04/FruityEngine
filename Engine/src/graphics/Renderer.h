#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Mesh.h"
#include "Framebuffer.h"
#include "effects/Scene.h"
#include "effects/RenderToScreen.h"


class Camera;
class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer() = default;

    void BeginFrame();
    void EndFrame();

    void SetClearColor(float r, float g, float b, float a);
    void Resize(int width, int height);

    void DrawMesh(const Mesh& mesh, std::shared_ptr<Shader> shader, const glm::mat4& modelMatrix, const Camera& camera);

    void SetPostProcessGraph(SceneEffect* screenNode, RenderToScreenEffect* outputNode);
    void ClearPostProcessGraph();

    int GetWidth() const;
    int GetHeight() const;

private:
    int _width, _height;
    glm::vec4 _clearColor;

    Framebuffer _hdrBuffer;

    RenderToScreenEffect* _postProcessOutput = nullptr;
};
