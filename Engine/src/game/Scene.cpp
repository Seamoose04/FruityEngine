#include "Scene.h"
#include <iostream>
#include <memory>
#include "core/FileLoaders/JSONLoader.h"
#include "game/Properties/Camera/Camera.h"

using json = nlohmann::json;

std::shared_ptr<Scene> Scene::LoadFromFile(const std::string& path) {
    auto scene = std::make_shared<Scene>();
    scene->_path = std::filesystem::path(path).parent_path().string();

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
			scene->_objects.push_back(scene->Create(objectJson));
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
	std::abort();
}

std::string Scene::GetPath() const {
	return _path;
}

Flags<SceneFlags> &Scene::GetFlags() {
	return _flags;
}

const std::vector<std::shared_ptr<GameObject>>& Scene::GetRootObjects() const {
	return _objects;
}

Renderer& Scene::GetRenderer() {
	return *_renderer;
}

void Scene::SetFlag(SceneFlags flag) {
	_flags.AddFlag(flag);
}

void Scene::ClearFlag(SceneFlags flag) {
	_flags.ClearFlag(flag);
}

void Scene::Init(int width, int height) {
	_renderer = std::make_unique<Renderer>(width, height);
}

void Scene::Start() {
	auto weakSelf = weak_from_this();
    for (auto& obj : _objects) {
        obj->OnCreate(weakSelf);
	}
}

void Scene::Update(float deltaTime) {
    for (auto& obj : _objects) {
        obj->Update(deltaTime);
	}
}

void Scene::HandleInput(const Window& window, float dt) {
    for (auto& obj : _objects) {
        obj->HandleInput(window, dt);
	}
}

void Scene::Render() {
	_renderer->BeginFrame();
	GetCamera().UpdateView();

    for (auto& obj : _objects) {
        obj->Render(GetRenderer());
	}

	_renderer->EndFrame();
}

void Scene::Resize(int width, int height) {
	for (auto& obj : _objects) {
		obj->OnResize(width, height);
	}

	_renderer->Resize(width, height);
}

void Scene::Unload() {
    for (auto& obj : _objects) {
        obj->OnDestroy();
    }

    _objects.clear();
}

std::shared_ptr<GameObject> Scene::Create(const json& chunk) {
	auto gameObject = std::make_shared<GameObject>();
	gameObject->FromJSON(chunk);
	if (chunk.contains("children")) {
		for (auto& childData : chunk["children"]) {
			std::shared_ptr<GameObject> child = Create(childData);
			gameObject->AddChild(child);
		}
	}
	return gameObject;
}

GameObject* Scene::Instantiate(const json& chunk, GameObject* parent) {
	std::shared_ptr<GameObject> go = Create(chunk);
	if (parent) {
		parent->AddChild(go);
	}
	go->OnCreate(weak_from_this());
	return go.get();
}

GameObject* Scene::Instantiate(const json& chunk) {
	return Instantiate(chunk, nullptr);
}

GameObject* Scene::FindByName(const std::string& name) const {
	auto it = std::find_if(_objects.begin(), _objects.end(), [&](const std::shared_ptr<GameObject>& object) {
		return object->GetName() == name;
	});
	if (it == _objects.end()) {
		return nullptr;
	}
	return it->get();
}

GameObject* Scene::FindByPath(const std::string& path) const {
	size_t slash = path.find('/');
	std::string before = path.substr(0, slash);
	std::string after = (slash != std::string::npos) ? path.substr(slash + 1) : "";
	
	GameObject* object = FindByName(before);
	if (!object || after.empty()) {
		return object;
	}
	return object->GetChildByPath(after);
}
