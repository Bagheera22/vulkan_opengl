#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
class Device;
class RenderPass;

class GraphicsPipeline
{
public:
	void Create(const Device& device, const VkExtent2D& swapChainExtent, const RenderPass& renderPass);
	void Destroy();
	operator const VkPipeline();
	operator const VkPipelineLayout() const;
private:
	VkShaderModule CreateShaderModule(const Device& device, const std::vector<char>& code) const;

	static std::vector<char> readFile(const std::string& filename);
	
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	const Device* m_device = nullptr;
};