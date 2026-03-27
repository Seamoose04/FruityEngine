#pragma once

#include "util/Flags.h"
#include "util/Observable.h"
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
	bool IsMouseButtonPressed(int button) const;

	Observable<glm::vec2> size;

private:
	GLFWwindow *_handle = nullptr;
	glm::vec2 _mousePos = {0, 0};
	
	Flags<WindowFlags> _flags;
};
