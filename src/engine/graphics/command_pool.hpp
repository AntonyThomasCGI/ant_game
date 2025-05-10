
#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "logical_device.hpp"
#include "surface.hpp"


class CommandPool
{
public:
    CommandPool(VmaAllocator &allocator, PhysicalDevice &physicalDevice, Device &device, Surface &surface);
    ~CommandPool();

    VkCommandPool const getCommandPool() const { return commandPool; }

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void createBuffer2(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& allocation);

    VmaAllocator &allocator;

private:
    VkCommandPool commandPool;

    Device &device;
};
