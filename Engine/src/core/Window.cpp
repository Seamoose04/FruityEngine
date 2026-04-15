#include "Window.h"
#include "GLFW/glfw3.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

Window::Window(int w, int h, const std::string& title) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		std::exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_handle = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
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
		self->onResize.Call(glm::vec2(w, h));
	});
	glfwSetCharCallback(_handle, [](GLFWwindow* window, unsigned int codepoint) {
		auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->onChar.Call(codepoint);
	});
	ShowCursor();
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window() {
	glfwDestroyWindow(_handle);
	glfwTerminate();
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(_handle);
}

void Window::PollEvents() {
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		_prevKeys[i] = glfwGetKey(_handle, i);
	}
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		_prevMouseButtons[i] = glfwGetMouseButton(_handle, i);
	}
    glfwPollEvents();

	double x, y;
	glfwGetCursorPos(_handle, &x, &y);
	_mousePos = glm::vec2((float)x, (float)y);
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

glm::vec2 Window::GetMousePos() const {
	return _mousePos;
}

bool Window::IsKeyPressed(int key) const {
    return glfwGetKey(_handle, key) == GLFW_PRESS;
}

bool Window::IsKeyJustPressed(int key) const {
	return IsKeyPressed(key) && _prevKeys[key] == GLFW_RELEASE;
}

bool Window::IsMouseButtonPressed(int button) const {
	return glfwGetMouseButton(_handle, button) == GLFW_PRESS;
}

bool Window::IsMouseButtonJustPressed(int button) const {
	return IsMouseButtonPressed(button) && _prevMouseButtons[button] == GLFW_RELEASE;
}
