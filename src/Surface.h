#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GLFWwindowWrapper;
class VulkanIntance;
class Surface
{
public:
	void Create(const GLFWwindowWrapper& window, const VulkanIntance& instance);

	void Destroy();
private:
	VkSurfaceKHR surface;
	const VulkanIntance* m_instance = nullptr;
	friend class Device;
	friend class SwapChain;
};
