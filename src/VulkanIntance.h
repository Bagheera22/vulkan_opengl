#pragma once
#include <vulkan/vulkan.hpp>


class VulkanIntance
{


public:
	void Create();
	void Destroy();
private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
	VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	void SetupDebugCallback();
	std::vector<const char*> GetRequiredExtensions();
	
	VkInstance instance = nullptr;
	VkDebugReportCallbackEXT callback;

	friend class Surface;
	friend class Device;
};
