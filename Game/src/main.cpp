#include <filesystem>
#include "core/App.h"
#include "game/Scene.h"

int main(int argc, char* argv[]) {
    std::filesystem::current_path(std::filesystem::absolute(argv[0]).parent_path());
    App app(1920, 1080, "4D Game");
    app.SetScene(Scene::LoadFromFile("assets/scenes/TestScene/scene.json"));
    app.Run();
    return EXIT_SUCCESS;
}
