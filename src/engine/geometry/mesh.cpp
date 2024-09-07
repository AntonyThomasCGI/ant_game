
#include <iostream>
#include <stdexcept>

#include "mesh.hpp"


Mesh::Mesh(LogicalDevice *logicalDevice, std::vector<Vertex> vertices, std::vector<uint16_t> indicies)
    : logicalDevice(logicalDevice), vertices(vertices), indicies(indicies)
{
}


Mesh::~Mesh()
{
    vkDestroyBuffer(logicalDevice->getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(logicalDevice->getDevice(), vertexBufferMemory, nullptr);
    vkDestroyBuffer(logicalDevice->getDevice(), indexBuffer, nullptr);
    vkFreeMemory(logicalDevice->getDevice(), indexBufferMemory, nullptr);
}


void Mesh::createBuffers(CommandPool &commandPool)
{
    createVertexBuffer(commandPool);

    if (!indicies.empty()) {
        createIndexBuffer(commandPool);
    }

}


void Mesh::createVertexBuffer(CommandPool &commandPool)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    commandPool.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory);

    void* data;
    vkMapMemory(logicalDevice->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(logicalDevice->getDevice(), stagingBufferMemory);

    commandPool.createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer,
        vertexBufferMemory);

    commandPool.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(logicalDevice->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice->getDevice(), stagingBufferMemory, nullptr);
}


void Mesh::createIndexBuffer(CommandPool &commandPool)
{
    VkDeviceSize bufferSize = sizeof(indicies[0]) * indicies.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    commandPool.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicalDevice->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indicies.data(), (size_t) bufferSize);
    vkUnmapMemory(logicalDevice->getDevice(), stagingBufferMemory);

    commandPool.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    commandPool.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(logicalDevice->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice->getDevice(), stagingBufferMemory, nullptr);
}


void Mesh::draw(CommandBuffer &commandBuffer, SwapChain &swapChain)
{

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer.getCommandBuffer(), 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer.getCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT16);


    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain.swapChainExtent.width);
    viewport.height = static_cast<float>(swapChain.swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer.getCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChain.swapChainExtent;
    vkCmdSetScissor(commandBuffer.getCommandBuffer(), 0, 1, &scissor);

    vkCmdDrawIndexed(commandBuffer.getCommandBuffer(), static_cast<uint32_t>(indicies.size()), 1, 0, 0, 0);

}
