#include "core/App.h"
#include "game/Scene.h"
#include <iostream>

int main() {
    App app(1280, 720, "4D Game");
    app.SetScene(Scene::LoadFromFile("Game/assets/scenes/TestScene/scene.json"));
    app.Run();
    return EXIT_SUCCESS;
}
