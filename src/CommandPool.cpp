#include "CommandPool.h"
#include "Device.h"

void CommandPool::Create(const Device& device)
{
	
	Device::QueueFamilyIndices queueFamilyIndices = device.GetQueueFamilyIndices(); //findQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

	if (vkCreateCommandPool(device.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
	m_device = &device;
}

void CommandPool::Destroy()
{
	vkDestroyCommandPool(m_device->device, commandPool, nullptr);
}

CommandPool::operator const VkCommandPool()
{
	return commandPool;
}
