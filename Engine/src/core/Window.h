#pragma once

#include "util/Flags.h"
#include "util/Initialized.h"
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
	void PollEvents() const;
	void SwapBuffers() const;
	void SetVSync(bool enabled);
	void SetTitle(const std::string &title);
	void GetSize(int &width, int &height) const;

	void HideCursor();
	void ShowCursor();
	void LockCursor();
	void DisableCursor();
	void DisableCursorVNC();
	void SetMouseRaw(bool raw);

	GLFWwindow *GetHandle() const { return _handle; }

	bool IsKeyPressed(int key) const;
	glm::vec2 GetMousePos() const;
	void SetMousePos(glm::vec2 pos);
	void ResetMousePos();

	Observable<glm::vec2> size;
	Observable<glm::vec2> center;

private:
	GLFWwindow *_handle = nullptr;
	Initialized<glm::vec2> _lastCursorPos;
	
	glm::vec2 _cursorPos{0};
	Flags<WindowFlags> _flags;
};
