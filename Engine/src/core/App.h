#pragma once

#include <string>
#include <memory>
#include "core/Window.h"
#include "core/Timer.h"
#include "game/Scene.h"
#include "util/Callback.h"

class App {
public:
	App(int width, int height, const std::string& title);
	~App();

	void Run();
	void SetScene(std::shared_ptr<Scene> newScene);
	Scene* GetCurrentScene() const;

	void ProcessSceneFlags(Flags<SceneFlags> &flags);
	Window& GetWindow();

	Callback<> postRender;

private:
	Window _window;
	Timer _timer;
	std::shared_ptr<Scene> _currentScene;
	bool _running = true;
};
