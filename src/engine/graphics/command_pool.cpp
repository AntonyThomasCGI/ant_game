
#include <stdexcept>


#include "command_pool.hpp"


CommandPool::CommandPool(Instance &instance, PhysicalDevice &physicalDevice, LogicalDevice &logicalDevice, Surface &surface) : logicalDevice(logicalDevice)
{
    QueueFamilyIndices queueFamilyIndices = surface.findQueueFamilies(physicalDevice.getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(logicalDevice.getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }

    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = physicalDevice.getPhysicalDevice();
    allocatorCreateInfo.device = logicalDevice.getDevice();
    allocatorCreateInfo.instance = instance.getInstance();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorCreateInfo, &allocator);
}


CommandPool::~CommandPool()
{

    vkDestroyCommandPool(logicalDevice.getDevice(), commandPool, nullptr);

    vmaDestroyAllocator(allocator);
}


VkCommandBuffer CommandPool::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    // It would be better to create a separate command pool specifically for these short-lived
    // buffers, the implementation may be able to apply memory allocation optimizations.
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice.getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicalDevice.getGraphicsQueue());

    vkFreeCommandBuffers(logicalDevice.getDevice(), commandPool, 1, &commandBuffer);
}


void CommandPool::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;  // Optional
    copyRegion.dstOffset = 0;  // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}


void CommandPool::createBuffer2(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& allocation) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo vmaAllocInfo = {};
    vmaAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;

    // TODO, pretty sure this is hard coded to one type of buffer creation
    vmaAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
        VMA_ALLOCATION_CREATE_MAPPED_BIT;


    vmaCreateBuffer(allocator, &bufferInfo, &vmaAllocInfo, &buffer, &allocation, nullptr);
}


void CommandPool::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    //VmaAllocationCreateInfo vmaAllocInfo = {};
    //vmaAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;

    //VmaAllocation allocation;
    //vmaCreateBuffer(allocator, &bufferInfo, &vmaAllocInfo, &buffer, &allocation, nullptr);

    if (vkCreateBuffer(logicalDevice.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice.getDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;

    PhysicalDevice physicalDevice = logicalDevice.getPhysicalDevice();
    allocInfo.memoryTypeIndex = physicalDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

    // A "real world" application shouldn't make an individual allocate call for every buffer.
    // You would use a custom allocator that splits up a single allocation to many objects using offsets.
    if (vkAllocateMemory(logicalDevice.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    if (vkBindBufferMemory(logicalDevice.getDevice(), buffer, bufferMemory, 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to bind buffer memory!");
    };
}
