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
	ReloadScene = 1 << 4,
	Quit = 1 << 5
};

class Scene : public std::enable_shared_from_this<Scene> {
public:
    Scene() = default;
    ~Scene() = default;

    static std::shared_ptr<Scene> LoadFromFile(const std::string& path);
    
	void Init(int width, int height);
    void Start();
    void Update(float dt);
    void Unload();
    void HandleInput(const Window& window, float dt);
    void Render();
	void Resize(int width, int height);

	void SetCamera(std::weak_ptr<Camera> camera);
	Camera& GetCamera() const;
	void SetFlag(SceneFlags flag);
	void ClearFlag(SceneFlags flag);
	Flags<SceneFlags> &GetFlags();
	const std::vector<std::shared_ptr<GameObject>>& GetRootObjects() const;
	Renderer& GetRenderer();

	std::string GetPath() const;

private:
	std::unique_ptr<Renderer> _renderer;
	std::weak_ptr<Camera> _camera;
    std::vector<std::shared_ptr<GameObject>> _objects;
    std::string _path;
	Flags<SceneFlags> _flags;
};
