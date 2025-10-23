#include "Window.h"
#include <glad/glad.h>
#include <iostream>

Window::Window(int w, int h, const std::string& title)
	: _width(w), _height(h)
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		std::exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_handle = glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
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

	glfwSetFramebufferSizeCallback(_handle, [](GLFWwindow*, int w, int h) {
		glViewport(0, 0, w, h);
	});
	
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

bool Window::IsKeyPressed(int key) const {
    return glfwGetKey(_handle, key) == GLFW_PRESS;
}

double Window::GetMouseX() const {
    double x, y;
    glfwGetCursorPos(_handle, &x, &y);
    return x;
}

double Window::GetMouseY() const {
    double x, y;
    glfwGetCursorPos(_handle, &x, &y);
    return y;
}