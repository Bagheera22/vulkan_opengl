#include "Framebuffers.h"

#include "RenderPass.h"
#include "Device.h"


void Framebuffers::Create(const Device& device, const VkExtent2D& swapChainExtent, const std::vector<VkImageView>& swapChainImageViews, const RenderPass& renderPass)
{
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass.renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device.device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
	m_device = &device;
}

void Framebuffers::Destroy()
{
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
		vkDestroyFramebuffer(m_device->device, swapChainFramebuffers[i], nullptr);
	}

}
std::size_t Framebuffers::size()
{
	return swapChainFramebuffers.size();
}
const VkFramebuffer& Framebuffers::operator[](std::size_t i ) const
{
	return swapChainFramebuffers[i];
}