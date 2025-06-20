

#include "context.hpp"


GraphicsContext::GraphicsContext(Window &window)
    : window(window)
{
    instance = std::make_unique<Instance>(window);
    surface = std::make_unique<Surface>(*instance, window);
    physicalDevice = std::make_unique<PhysicalDevice>(*instance, *surface);
    physicalDevice->pickPhysicalDevice();
    device = std::make_unique<Device>(*instance, *surface, *physicalDevice);

    // Create the allocator
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = physicalDevice->getPhysicalDevice();

    allocatorCreateInfo.device = device->getDevice();
    allocatorCreateInfo.instance = instance->getInstance();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    // End creating allocator

}


GraphicsContext::~GraphicsContext()
{
}
