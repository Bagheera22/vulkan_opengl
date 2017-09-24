//https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Rendering_and_presentation

//https://vulkan-tutorial.com/code/index_buffer.cpp

//https://vulkan-tutorial.com/code/model_loading.cpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>

#include "VulkanIntance.h"
#include "Device.h"
#include "Surface.h"
#include "SwapChain.h"
#include "VulkanIntance.h"
#include "GLFWwindowWrapper.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "Framebuffers.h"
#include "CommandPool.h"
#include "Vertex.h"
#include "Buffer.h"




class Semaphore
{
public:
	void Create(const Device& device)
	{
		m_device = &device;
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_semaphore) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create semaphores!");
		}
	}
	operator const VkSemaphore() const
	{
		return m_semaphore;
	}
	void Destroy()
	{
		vkDestroySemaphore(*m_device, m_semaphore, nullptr);
	}
private:
	VkSemaphore m_semaphore = 0;
	const Device* m_device = nullptr;
};



//#ifdef NDEBUG
const bool enableValidationLayers = false;
//#else
//const bool enableValidationLayers = true;
//#endif


class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindowWrapper window;
	VulkanIntance instance;
	Surface surface;
	Device device;
	SwapChain swapChain;
	RenderPass renderPass;
	GraphicsPipeline graphicsPipeline;
	Framebuffers swapChainFramebuffers;
	CommandPool commandPool;
	std::unique_ptr<Buffer> vertexBuffer;
	std::unique_ptr<Buffer> indexBuffer;


	std::vector<VkCommandBuffer> commandBuffers;

	Semaphore imageAvailableSemaphore;
	Semaphore renderFinishedSemaphore;

	void initWindow() {
		window.InitGLF();
	}

	void initVulkan() {
		instance.Create();
		surface.Create(window, instance);
		device.Create(instance, surface);
		swapChain.Create(device, surface);
		renderPass.Create(device, swapChain.GetSwapChainImageFormat());
		graphicsPipeline.Create(device, swapChain.GetSwapChainExtent(), renderPass);
		swapChainFramebuffers.Create(device, swapChain.GetSwapChainExtent(), swapChain.GetSwapChainImageViews(), renderPass);
		commandPool.Create(device);


		const std::vector<Vertex> vertices = {
			{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
			{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
			{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
			{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
		};

		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};
		vertexBuffer = std::make_unique<Buffer>(vertices, device, commandPool);
		indexBuffer = std::make_unique<Buffer>(indices, device, commandPool);

		createCommandBuffers();
		//createCommandBuffers();

		createSemaphores();
	}

	void mainLoop() {
		while (!window.ShouldClose()) {
			glfwPollEvents();
			drawFrame();
		}
		device.WaitIdle();
	}

	void cleanup() {

		indexBuffer.release();
		vertexBuffer.release();

		renderFinishedSemaphore.Destroy();
		imageAvailableSemaphore.Destroy();

		commandPool.Destroy();
		swapChainFramebuffers.Destroy();
		graphicsPipeline.Destroy();
		renderPass.Destroy();
		swapChain.Destroy();
		device.Destroy();
		surface.Destroy();
		instance.Destroy();
		window.DestroyAndTerminate();
	}


	


	void createCommandBuffers() {
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChain.GetSwapChainExtent();// swapChainExtent;

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			VkBuffer vertexBuffers[] = { *vertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffers[i], *indexBuffer, 0, VK_INDEX_TYPE_UINT16);

			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indexBuffer->size()), 1, 0, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		/*VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChain.GetSwapChainExtent();// swapChainExtent;

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}*/
	}

	void createSemaphores() {
		renderFinishedSemaphore.Create(device);
		imageAvailableSemaphore.Create(device);
	}

	void drawFrame() {
		uint32_t imageIndex;
		vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(device.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(device.GetPresentQueue(), &presentInfo);

		vkQueueWaitIdle(device.GetPresentQueue());
	}


	
	



	/*bool checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (VkLayerProperties layerName : availableLayers) {
			std::cout << layerName.layerName << "\n";
			std::cout << layerName.description <<"\n";
		}
		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}*/



};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}