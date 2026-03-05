#pragma once

#include "Shader.h"
#include "Framebuffer.h"
#include "util/Registry.h"
#include <json.hpp>
#include <unordered_map>
#include <string>

using json = nlohmann::json;

class PostProcessEffect {
public:
    PostProcessEffect() = default;
    virtual ~PostProcessEffect() = default;

    virtual void Init(int width, int height) = 0;
    virtual void Execute() = 0;
    virtual void Resize(int width, int height);
    virtual void FromJSON(const json& data) {}
    void SetInput(const std::string& slot, PostProcessEffect* effect);
    PostProcessEffect* GetInput(const std::string& slot) const;

    virtual Framebuffer* GetOutput();
    static void InitQuad();

protected:
    Framebuffer _output;
    std::unordered_map<std::string, PostProcessEffect*> _inputs;

    static void DrawQuad();

private:
    static unsigned int _quadVAO;
    static unsigned int _quadVBO;
    static bool _quadInitialized;
};

#define REGISTER_EFFECT(DerivedType) REGISTER_TYPE(PostProcessEffect, DerivedType)