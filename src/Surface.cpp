#include "Surface.h"
#include "GLFWwindowWrapper.h"
#include "VulkanIntance.h"

void Surface::Create(const GLFWwindowWrapper& window, const VulkanIntance& instance)
{
	if (glfwCreateWindowSurface(instance.instance, window.window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
	m_instance = &instance;
}
void Surface::Destroy()
{
	vkDestroySurfaceKHR(m_instance->instance, surface, nullptr);
}