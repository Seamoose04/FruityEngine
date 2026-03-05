#include "Framebuffer.h"

#include <iostream>

Framebuffer::Framebuffer(int width, int height, GLenum colorFormat)
    : _width(width), _height(height), _colorFormat(colorFormat)
{
    Setup();
}

Framebuffer::~Framebuffer() {
    Destroy();
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
    : _fbo(other._fbo), _colorTexture(other._colorTexture), _depthRBO(other._depthRBO),
      _width(other._width), _height(other._height), _colorFormat(other._colorFormat)
{
    other._fbo = 0;
    other._colorTexture = 0;
    other._depthRBO = 0;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
    if (this != &other) {
        Destroy();
        _fbo          = other._fbo;
        _colorTexture = other._colorTexture;
        _depthRBO     = other._depthRBO;
        _width        = other._width;
        _height       = other._height;
        _colorFormat  = other._colorFormat;
        other._fbo = other._colorTexture = other._depthRBO = 0;
    }
    return *this;
}

void Framebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _width, _height);
}

void Framebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(int width, int height) {
    _width = width;
    _height = height;
    Destroy();
    Setup();
}

unsigned int Framebuffer::GetColorTexture() const {
    return _colorTexture;
}

bool Framebuffer::IsValid() const {
    return _fbo != 0;
}

void Framebuffer::Setup() {
    // Texture(color) - floats for HDR support (>1.0)
    glGenTextures(1, &_colorTexture);
    glBindTexture(GL_TEXTURE_2D, _colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, _colorFormat, _width, _height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Clamp edges for blurs
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Depth - Renderbuffer since we never sample it
    glGenRenderbuffers(1, &_depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Assemble FBO
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer incomplete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Destroy() {
    if (_colorTexture) {
        glDeleteTextures(1, &_colorTexture);
        _colorTexture = 0;
    }
    if (_depthRBO) {
        glDeleteRenderbuffers(1, &_depthRBO);
        _depthRBO = 0;
    }
    if (_fbo) {
        glDeleteFramebuffers(1, &_fbo);
        _fbo = 0;
    }
}