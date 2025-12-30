#include "Scene.h"
#include <iostream>
#include "core/FileLoaders/JSONLoader.h"
#include "game/Properties/Camera.h"

using json = nlohmann::json;

std::shared_ptr<Scene> Scene::LoadFromFile(const std::string& path) {
    auto scene = std::make_shared<Scene>();
    scene->_path = std::filesystem::path(path).parent_path();

    JSONLoader loader;
    json sceneJson = *static_cast<json*>(loader.Load(path));

    if (!sceneJson.contains("objects") || !sceneJson["objects"].is_array()) {
        std::cerr << "[Scene] Invalid scene file: missing 'objects' array\n";
        return scene;
    }

    for (const auto& objPath : sceneJson["objects"]) {
        if (!objPath.is_string()) continue;

        std::filesystem::path fullPath = scene->_path + "/" + objPath.get<std::string>();
        if (!std::filesystem::exists(fullPath)) {
            std::cerr << "[Scene] Missing GameObject file: " << fullPath << "\n";
            continue;
        }

        try {
            json objectJson = *static_cast<json*>(loader.Load(fullPath.string()));
            auto gameObject = std::make_shared<GameObject>();
            gameObject->FromJSON(objectJson, scene);
            scene->_objects.push_back(gameObject);
        }
        catch (const std::exception& e) {
            std::cerr << "[Scene] Error loading GameObject from " << fullPath << ": " << e.what() << "\n";
        }
    }

    return scene;
}

void Scene::SetCamera(std::weak_ptr<Camera> camera) {
	_camera = camera;
}

Camera &Scene::GetCamera() const {
	if (auto lockedCamera = _camera.lock()) {
		return *(Camera*)lockedCamera.get();
	}
	std::cerr << "No camera in scene" << std::endl;
}

std::string Scene::GetPath() const {
	return _path;
}

Flags<SceneFlags> &Scene::GetFlags() {
	return _flags;
}

void Scene::SetFlag(SceneFlags flag) {
	_flags.AddFlag(flag);
}

void Scene::ClearFlag(SceneFlags flag) {
	_flags.ClearFlag(flag);
}

void Scene::Start() {
    for (auto& obj : _objects)
        obj->OnCreate();
}

void Scene::Update(float deltaTime) {
    for (auto& obj : _objects)
        obj->Update(deltaTime);
}

void Scene::HandleInput(const Window& window, float dt) {
    for (auto& obj : _objects)
        obj->HandleInput(window, dt);
}

void Scene::Render(Renderer &renderer) {
	GetCamera().UpdateView();
    for (auto& obj : _objects)
        obj->Render(renderer);
}

void Scene::Unload() {
    for (auto& obj : _objects) {
        obj->OnDestroy();
    }
    _objects.clear();
}
