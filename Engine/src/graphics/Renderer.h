#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include "Mesh.h"

class Camera;
class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();

    void BeginFrame();
    void EndFrame();

    void ClearColor(float r, float g, float b, float a);
    void Resize(int width, int height);

    void DrawMesh(const Mesh& mesh, const Shader& shader, const glm::mat4& modelMatrix, const Camera& camera);

private:
    int _width, _height;
    glm::vec4 _clearColor;
};
