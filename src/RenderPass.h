#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Device;

class RenderPass
{
public:
	void Create(const Device& device, VkFormat format);
	void Destroy();
	operator const VkRenderPass();
private:
	VkRenderPass renderPass;
	const Device* m_device;

	friend class GraphicsPipeline;
	friend class Framebuffers;
};