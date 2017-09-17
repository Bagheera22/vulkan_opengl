#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GLFWwindowWrapper
{
public:
	void InitGLF()
	{
		constexpr int WIDTH = 800;
		constexpr int HEIGHT = 600;

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}
	bool ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}
	void DestroyAndTerminate()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
private:
	GLFWwindow* window = nullptr;
	friend class Surface;
};
