#include "App.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

App::App(int width, int height, const std::string& title)
    : _window(width, height, title), _renderer(width, height)
{
    _window.SetVSync(true);
    std::cout << "App initialized successfully.\n";
}

App::~App() {
    if (_currentScene) _currentScene->Unload();
    std::cout << "App shutting down.\n";
}

void App::Run() {
    while (!_window.ShouldClose() && _running) {
        float dt = _timer.GetDeltaTime();

        if (_currentScene) {
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
    if (_currentScene) _currentScene->Start();
}

void App::ProcessInput(float dt) {
    if (_window.IsKeyPressed(GLFW_KEY_ESCAPE)) {
        _running = false;
    }
    _currentScene->HandleInput(_window, dt);
}

void App::Update(float dt) {
    _currentScene->Update(dt);
}

void App::Render() {
    _renderer.BeginFrame();
    _currentScene->Render(_renderer);
    _renderer.EndFrame();
}
