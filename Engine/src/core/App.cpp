#include "App.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game/Properties/Camera.h"
#include "game/Scene.h"

App::App(int width, int height, const std::string& title)
    : _window(width, height, title), _renderer(width, height)
{
    _window.SetVSync(true);

	_window.size.OnChange([this](glm::vec2 size) {
		_renderer.Resize(size.x, size.y);
		_currentScene->GetCamera().UpdateAspectRatio(size.x, size.y);
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

            _renderer.BeginFrame();
            _currentScene->Render(_renderer);
            _renderer.EndFrame();
        }
        
        _window.SwapBuffers();
        _window.PollEvents();
    }
}

void App::SetScene(std::shared_ptr<Scene> newScene) {
    if (_currentScene) _currentScene->Unload();
    _currentScene = std::move(newScene);
	std::cout << "starting scene" << std::endl;
    if (_currentScene) _currentScene->Start();
	std::cout << "scene started" << std::endl;
}

void App::ProcessSceneFlags(Flags<SceneFlags> &flags) {
	if (flags.ProcessFlag(SceneFlags::CursorNormal)) {
		_window.ShowCursor();
	}
	if (flags.CheckFlag(SceneFlags::CursorHidden) && flags.CheckFlag(SceneFlags::CursorLocked) && flags.CheckFlag(SceneFlags::VNC)) {
		flags.ClearFlag(SceneFlags::CursorHidden);
		flags.ClearFlag(SceneFlags::CursorLocked);
		_window.DisableCursorVNC();
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
	if (flags.ProcessFlag(SceneFlags::ResetCursor)) {
		_window.ResetMousePos();
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
