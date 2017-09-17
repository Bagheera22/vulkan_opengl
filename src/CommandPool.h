#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Device;

class CommandPool
{
public:
	void Create(const Device& device);
	void Destroy();
	operator const VkCommandPool();
private:
	VkCommandPool commandPool;
	const Device* m_device = nullptr;

};

