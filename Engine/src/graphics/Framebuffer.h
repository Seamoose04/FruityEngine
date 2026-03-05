#pragma once

#include <glad/glad.h>

class Framebuffer {
public:
    Framebuffer() = default;
    Framebuffer(int width, int height, GLenum colorFormat = GL_RGBA16F);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    void Bind() const;
    void Unbind() const;
    void Resize(int width, int height);

    unsigned int GetColorTexture() const;
    bool IsValid() const;

private:
    unsigned int _fbo = 0;
    unsigned int _colorTexture = 0;
    unsigned int _depthRBO = 0;

    int _width = 0;
    int _height = 0;
    GLenum _colorFormat = GL_RGBA16F;

    void Setup();
    void Destroy();
};