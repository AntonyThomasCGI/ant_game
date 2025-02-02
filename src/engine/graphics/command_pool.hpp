
#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.hpp"
#include "surface.hpp"


class CommandPool
{
public:
    CommandPool(PhysicalDevice &physicalDevice, LogicalDevice &logicalDevice, Surface &surface);
    ~CommandPool();

    VkCommandPool const getCommandPool() const { return commandPool; }

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

private:
    VkCommandPool commandPool;

    LogicalDevice &logicalDevice;
};
