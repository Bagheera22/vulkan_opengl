#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
class Device;
class RenderPass;

#include <vector>

class Framebuffers
{
public:
	void Create(const Device& device, const VkExtent2D& swapChainExtent, const std::vector<VkImageView>& swapChainImageViews, const RenderPass& renderPass);
	void Destroy();
	std::size_t size();
	const VkFramebuffer& operator[](std::size_t i ) const;
private:
	std::vector<VkFramebuffer> swapChainFramebuffers;
	const Device* m_device = nullptr;
};