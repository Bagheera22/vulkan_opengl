#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Vertex.h"

#include <vector>

class Device;
class CommandPool;

class Buffer
{
public:
	Buffer(const std::vector<uint16_t>& indexes, const Device& device, const CommandPool& commandPool);
	Buffer(const std::vector<Vertex>& vertices, const Device& device, const CommandPool& commandPool);
	~Buffer();
	inline operator VkBuffer() const { return m_buffer; }
	inline std::size_t size() { return m_size; }
private:
	void createVertexBuffer(const std::vector<Vertex>& vertices, const CommandPool& commandPool);
	void createIndexBuffer(const std::vector<uint16_t>& indices, const CommandPool& commandPool);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const CommandPool& commandPool);

	const Device& m_device;
	VkBuffer m_buffer = 0;
	VkDeviceMemory m_bufferMemory = 0;
	std::size_t m_size = 0;
};
