#include "App.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game/Scene.h"

App::App(int width, int height, const std::string& title)
    : _window(width, height, title)
{
    _window.SetVSync(true);

	_window.size.OnChange([this](glm::vec2 size) {
		_currentScene->Resize(size.x, size.y);
	});
    std::cout << "App initialized successfully.\n";
}

App::~App() {
    if (_currentScene) _currentScene->Unload();
    std::cout << "App shutting down.\n";
}

void App::Run() {
	std::cout << "running!" << std::endl;
    while (!_window.ShouldClose() && _running) {
        float dt = _timer.GetDeltaTime();

        if (_currentScene) {
			ProcessSceneFlags(_currentScene->GetFlags());
            _currentScene->HandleInput(_window, dt);
            _currentScene->Update(dt);
            _currentScene->Render();
			postRender.Call();
        }
        
        _window.SwapBuffers();
        _window.PollEvents();
    }
}

void App::SetScene(std::shared_ptr<Scene> newScene) {
    if (_currentScene) {
		_currentScene->Unload();
	}
    _currentScene = std::move(newScene);
	if (_currentScene) {
		std::cout << "starting scene" << std::endl;
		_currentScene->Init(_window.size->x, _window.size->y);
    	_currentScene->Start();
		std::cout << "scene started" << std::endl;
	}
}

void App::ProcessSceneFlags(Flags<SceneFlags> &flags) {
	if (flags.ProcessFlag(SceneFlags::CursorNormal)) {
		_window.ShowCursor();
	}
	if (flags.CheckFlag(SceneFlags::CursorHidden) && flags.CheckFlag(SceneFlags::CursorLocked)) {
		flags.ClearFlag(SceneFlags::CursorHidden);
		flags.ClearFlag(SceneFlags::CursorLocked);
		_window.DisableCursor();
	}
	if (flags.ProcessFlag(SceneFlags::CursorHidden)) {
		_window.HideCursor();
	}
	if (flags.ProcessFlag(SceneFlags::CursorLocked)) {
		_window.LockCursor();
	}
	if (flags.ProcessFlag(SceneFlags::CursorRaw)) {
		_window.SetMouseRaw(true);
	}
	if (flags.ProcessFlag(SceneFlags::ReloadScene)) {
		std::string path = _currentScene->GetPath();
		_currentScene->Unload();
		SetScene(Scene::LoadFromFile(path));
	}
	if (flags.ProcessFlag(SceneFlags::Quit)) {
		_running = false;
	}
}

Window& App::GetWindow() {
	return _window;
}

Scene* App::GetCurrentScene() const {
	return _currentScene.get();
}
