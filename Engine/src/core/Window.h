#pragma once

#include "util/Callback.h"
#include "util/Flags.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

enum class WindowFlags : unsigned long {
	None = 0,
	CursorAbsolute = 1 << 0,
	CursorRelative = 1 << 1
};

class Window {
public:
	Window(int width, int height, const std::string &title);
	~Window();

	bool ShouldClose() const;
	void PollEvents();
	void SwapBuffers() const;
	void SetVSync(bool enabled);
	void SetTitle(const std::string &title);
	void GetSize(int &width, int &height) const;

	void HideCursor();
	void ShowCursor();
	void LockCursor();
	void DisableCursor();
	void SetMouseRaw(bool raw);

	GLFWwindow *GetHandle() const { return _handle; }
	glm::vec2 GetMousePos() const;

	bool IsKeyPressed(int key) const;
	bool IsKeyJustPressed(int key) const;
	bool IsMouseButtonPressed(int button) const;
	bool IsMouseButtonJustPressed(int button) const;

	mutable Callback<glm::vec2> onResize;
	mutable Callback<unsigned int> onChar;

private:
	GLFWwindow *_handle = nullptr;
	glm::vec2 _mousePos = {0, 0};
	std::array<int, GLFW_KEY_LAST> _prevKeys{};
	std::array<int, GLFW_MOUSE_BUTTON_LAST> _prevMouseButtons{};

	Flags<WindowFlags> _flags;
};
