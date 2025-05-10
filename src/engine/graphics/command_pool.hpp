
#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "context.hpp"


class CommandPool
{
public:
    CommandPool(GraphicsContext &ctx);
    ~CommandPool();

    VkCommandPool const getCommandPool() const { return commandPool; }

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void createBuffer2(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& allocation);

private:
    GraphicsContext &ctx;

    VkCommandPool commandPool;
};
