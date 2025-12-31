#pragma once
#include "core/Window.h"
#include "graphics/Renderer.h"
#include "game/GameObject.h"
#include "util/Flags.h"
#include <memory>
#include <vector>

enum class SceneFlags : unsigned long {
	None = 0,
	CursorNormal = 1 << 0,
	CursorHidden = 1 << 1,
	CursorLocked = 1 << 2,
	CursorRaw = 1 << 3,
	ResetCursor = 1 << 4,
	ReloadScene = 1 << 5,
	VNC = 1 << 6,
	Quit = 1 << 7
};

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    static std::shared_ptr<Scene> LoadFromFile(const std::string& path);
    
    void Start();
    void Update(float dt);
    void Unload();
    void HandleInput(const Window& window, float dt);
    void Render(Renderer& renderer);
	void SetCamera(std::weak_ptr<Camera> camera);
	Flags<SceneFlags> &GetFlags();
	void SetFlag(SceneFlags flag);
	void ClearFlag(SceneFlags flag);
	Camera &GetCamera() const;

	std::string GetPath() const;

private:
	std::weak_ptr<Camera> _camera;
    std::vector<std::shared_ptr<GameObject>> _objects;
    std::string _path;
	Flags<SceneFlags> _flags;
};
