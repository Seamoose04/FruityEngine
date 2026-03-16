#include <filesystem>
#include "core/App.h"
#include "game/Scene.h"

int main(int argc, char* argv[]) {
	#ifdef _WIN32
		auto exePath = std::filesystem::absolute(argv[0]).parent_path();
	#else
		auto exePath = std::filesystem::canonical("/proc/self/exe").parent_path();
	#endif
	std::filesystem::current_path(exePath);

    App app(1920, 1080, "4D Game");
    app.SetScene(Scene::LoadFromFile("assets/scenes/my_scene/scene.json"));
    app.Run();
    return EXIT_SUCCESS;
}
