#pragma once

#include <string>
#include <memory>
#include "core/Window.h"
#include "graphics/Renderer.h"
#include "core/Timer.h"
#include "game/Scene.h"

class App {
public:
	App(int width, int height, const std::string& title);
	~App();

	void Run();
	void SetScene(std::shared_ptr<Scene> newScene);

	void ProcessSceneFlags(Flags<SceneFlags> &flags);

private:
	Window _window;
	Renderer _renderer;
	Timer _timer;
	std::shared_ptr<Scene> _currentScene;
	bool _running = true;
};
