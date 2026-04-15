#include <filesystem>
#include "core/App.h"
#include "game/Scene.h"

#include "EditorScene.h"

int main(int argc, char* argv[]) {
	#ifdef _WIN32
		auto exePath = std::filesystem::absolute(argv[0]).parent_path();
	#else
		auto exePath = std::filesystem::canonical("/proc/self/exe").parent_path();
	#endif
	std::filesystem::current_path(exePath);

	EditorScene::Instance().Load("../Game/assets/scenes/Monkey/scene.json");
    App app(1920, 1080, "Fruit Bowl");
    app.SetScene(Scene::LoadFromFile("assets/Editor.json"));
    app.Run();

    return EXIT_SUCCESS;
}
