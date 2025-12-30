#include "Window.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Window::Window(int w, int h, const std::string& title) : size({w, h}) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		std::exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_handle = glfwCreateWindow(size->x, size->y, title.c_str(), nullptr, nullptr);
	if (!_handle) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_handle);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD\n";
		std::exit(EXIT_FAILURE);
	}
	
	glfwSetWindowUserPointer(_handle, this);
	glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->size.Set({w, h});
	});
	glfwSetCursorPosCallback(_handle, [](GLFWwindow* window, double x, double y) {
		auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->SetMousePos(glm::vec2(x, y));
	});
	ShowCursor();
	
	glEnable(GL_DEPTH_TEST);
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window() {
	glfwDestroyWindow(_handle);
	glfwTerminate();
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(_handle);
}

void Window::PollEvents() const {
    glfwPollEvents();
}

void Window::SwapBuffers() const {
    glfwSwapBuffers(_handle);
}

void Window::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::SetTitle(const std::string& title) {
	glfwSetWindowTitle(_handle, title.c_str());
}

void Window::GetSize(int& w, int& h) const {
	glfwGetFramebufferSize(_handle, &w, &h);
}

void Window::HideCursor() {
	glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	_flags.ClearFlag(WindowFlags::CursorRelative);
	_flags.AddFlag(WindowFlags::CursorAbsolute);
}

void Window::ShowCursor() {
	glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	_flags.ClearFlag(WindowFlags::CursorRelative);
	_flags.AddFlag(WindowFlags::CursorAbsolute);
}

void Window::LockCursor() {
	glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
	_flags.ClearFlag(WindowFlags::CursorRelative);
	_flags.AddFlag(WindowFlags::CursorAbsolute);
}

void Window::DisableCursor() {
	glfwPollEvents();
	glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_flags.ClearFlag(WindowFlags::CursorAbsolute);
	_flags.AddFlag(WindowFlags::CursorRelative);
}

void Window::SetMouseRaw(bool raw) {
	if (raw) {
		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(_handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		} else {
			std::cerr << "raw mouse motion NOT supported" << std::endl;
		}
	} else {
		glfwSetInputMode(_handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
	}
}

bool Window::IsKeyPressed(int key) const {
    return glfwGetKey(_handle, key) == GLFW_PRESS;
}

glm::vec2 Window::GetMousePos() const {
	int w, h;
	GetSize(w, h);

	// double x, y;
	// glfwGetCursorPos(_handle, &x, &y);

	// return glm::vec2(x / w, y / h);
	return _cursorPos / glm::vec2(w, h);
}

void Window::SetMousePos(glm::vec2 pos) {
	if (_flags.CheckFlag(WindowFlags::CursorAbsolute)) {
		_cursorPos = pos;
		return;
	}
	if (_flags.CheckFlag(WindowFlags::CursorRelative)) {
		int w, h;
		GetSize(w, h);
		glm::vec2 centered = pos - glm::vec2(w / 2.0f, h / 2.0f);

		if (_lastCursorPos.IsInitialized()) {
			_cursorPos += centered - _lastCursorPos.Get();
			std::cout << (centered - _lastCursorPos.Get()).x << std::endl;
			_lastCursorPos.Reset();
			_lastCursorPos = centered;
		} else {
			std::cout << "initializing" << std::endl;
			_lastCursorPos = centered;
		}

		return;
	}
}

void Window::ResetMousePos() {
	_cursorPos = glm::vec2(0);
}

