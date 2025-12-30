#include "core/App.h"
#include "game/Scene.h"

int main() {
    App app(1920, 1080, "4D Game");
    app.SetScene(Scene::LoadFromFile("Game/assets/scenes/TestScene/scene.json"));
    app.Run();
    return EXIT_SUCCESS;
}
