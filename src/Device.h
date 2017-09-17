#pragma once
#include <set>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
class Surface;
class VulkanIntance;
class Device
{
public:
	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	void WaitIdle();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	void Create(const VulkanIntance& instance, const Surface& surface);
	void Destroy();

	inline const QueueFamilyIndices& GetQueueFamilyIndices()const { return indices; }
	operator VkDevice() const
	{
		return device;
	}
	
	inline VkQueue GetGraphicsQueue() const
	{
		return graphicsQueue;
	}

	inline VkQueue GetPresentQueue() const
	{
		return presentQueue;
	}
	inline VkPhysicalDevice GetPhysicalDevice() const
	{
		return physicalDevice;
	}
private:
	void PickPhysicalDevice(const VulkanIntance& instance, const Surface& surface);
	bool IsDeviceSuitable(VkPhysicalDevice device, const Surface& surface);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, const Surface& surface);

	VkDevice device = nullptr;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue = nullptr;
	VkQueue presentQueue = nullptr;
	QueueFamilyIndices indices;

	friend class SwapChain;
	friend class RenderPass;
	friend class GraphicsPipeline;
	friend class Framebuffers;
	friend class CommandPool;
};
