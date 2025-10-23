// #include "Player.h"
// #include <glm/gtc/matrix_transform.hpp>
// #include <GLFW/glfw3.h>

// void Player::HandleInput(const Window& window, float dt) {
//     glm::vec3 dir(0.0f);
//     if (window.IsKeyPressed(GLFW_KEY_W)) dir.z -= 1.0f;
//     if (window.IsKeyPressed(GLFW_KEY_S)) dir.z += 1.0f;
//     if (window.IsKeyPressed(GLFW_KEY_A)) dir.x -= 1.0f;
//     if (window.IsKeyPressed(GLFW_KEY_D)) dir.x += 1.0f;
//     position += glm::normalize(dir) * speed * dt;
// }

// void Player::Update(float dt) {
//     transform = glm::translate(glm::mat4(1.0f), position);
// }

// void Player::Render(Renderer& renderer) {
//     // placeholder: draw a cube, tetrahedron, etc. later
// }
