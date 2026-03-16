#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "graphics/Vertex.h"

class Mesh {
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    Mesh(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& other) noexcept;
    ~Mesh();

    void Bind() const;
    void Unbind() const;
    unsigned int GetIndexCount() const { return static_cast<unsigned int>(indices.size()); }

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void _SetupMesh();
};
