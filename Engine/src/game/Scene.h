#pragma once
#include "core/Window.h"
#include "graphics/Renderer.h"
#include "game/GameObject.h"
#include <memory>
#include <vector>

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    static std::shared_ptr<Scene> LoadFromFile(const std::string& path);
    
    void Start();
    void Update(float dt);
    void Unload();
    void HandleInput(const Window& window, float dt);
    void Render(Renderer& renderer);

    const std::vector<std::shared_ptr<GameObject>>& GetObjects() const { return _objects; }

private:
    std::vector<std::shared_ptr<GameObject>> _objects;
    std::string _scenePath;
};
