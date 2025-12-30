#include "Mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<glm::vec3>& verts, const std::vector<unsigned int>& inds)
    : vertices(verts), indices(inds)
{
	SetupMesh();
}

Mesh::Mesh(Mesh&& other) noexcept {
    vao = other.vao;
    vbo = other.vbo;
    ebo = other.ebo;
    vertices = std::move(other.vertices);
    indices = std::move(other.indices);
    other.vao = other.vbo = other.ebo = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (vao) glDeleteVertexArrays(1, &vao);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);

        vao = other.vao;
        vbo = other.vbo;
        ebo = other.ebo;
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        other.vao = other.vbo = other.ebo = 0;
    }
    return *this;
}

Mesh::~Mesh() {
    if (ebo) glDeleteBuffers(1, &ebo);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void Mesh::Bind() const {
    glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Mesh::Unbind() const {
    glBindVertexArray(0);
}