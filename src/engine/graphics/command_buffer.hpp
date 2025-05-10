#pragma once

#include <vulkan/vulkan.h>

#include "command_pool.hpp"
#include "logical_device.hpp"


class CommandBuffer
{
public:
    CommandBuffer();
    CommandBuffer(Device &device, CommandPool &commandPool);

    void start();
    void end();
    void submit(VkSemaphore &waitSemaphore, VkSemaphore &finishedSemaphore, VkFence fence);

    VkCommandBuffer &getCommandBuffer() { return commandBuffer; }

private:
    Device &device;
    VkCommandBuffer commandBuffer;
};
