#pragma once
#include <string>
#include "graphics/Renderer.h"
#include "core/Window.h"

class GameObject;

class Script {
public:
    GameObject* gameObject;

    virtual ~Script() = default;
    virtual void Start() {}
    virtual void Update(float dt) {}
    virtual void HandleInput(const Window& window, float dt) {}
    virtual void Render(Renderer& renderer) {}
    virtual void Destroy() {}
};
