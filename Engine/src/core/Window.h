#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
	Window(int width, int height, const std::string& title);
	~Window();

	bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;
    void SetVSync(bool enabled);
    void SetTitle(const std::string& title);
    void GetSize(int& width, int& height) const;

	GLFWwindow* GetHandle() const { return _handle; }

	bool IsKeyPressed(int key) const;
	double GetMouseX() const;
	double GetMouseY() const;

private:
	GLFWwindow* _handle = nullptr;
	int _width = 0, _height = 0;
};
