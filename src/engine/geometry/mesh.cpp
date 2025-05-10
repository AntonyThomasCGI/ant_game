

#include <iostream>
#include <stdexcept>

#include "mesh.hpp"


Mesh::Mesh(GraphicsContext &ctx, std::vector<Vertex> vertices, std::vector<uint16_t> indicies)
    : ctx(ctx), vertices(vertices), indicies(indicies)
{
}


Mesh::~Mesh()
{
    vkDestroyBuffer(ctx.device->getDevice(), vertexBuffer, nullptr);
    //vkFreeMemory(ctx.device->getDevice(), vertexBufferMemory, nullptr);
    vmaFreeMemory(ctx.allocator, vertexBufferMemory);
    vkDestroyBuffer(ctx.device->getDevice(), indexBuffer, nullptr);
    //vkFreeMemory(ctx.device->getDevice(), indexBufferMemory, nullptr);
    vmaFreeMemory(ctx.allocator, indexBufferMemory);
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
    VmaAllocation stagingBufferMemory;
    commandPool.createBuffer2(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory);

    void* data;
    //vkMapMemory(ctx.device->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    vmaMapMemory(ctx.allocator, stagingBufferMemory, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    //vkUnmapMemory(ctx.device->getDevice(), stagingBufferMemory);
    vmaUnmapMemory(ctx.allocator, stagingBufferMemory);

    commandPool.createBuffer2(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer,
        vertexBufferMemory);

    commandPool.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(ctx.device->getDevice(), stagingBuffer, nullptr);
    //vkFreeMemory(ctx.device->getDevice(), stagingBufferMemory, nullptr);
    vmaFreeMemory(ctx.allocator, stagingBufferMemory);
}


void Mesh::createIndexBuffer(CommandPool &commandPool)
{
    VkDeviceSize bufferSize = sizeof(indicies[0]) * indicies.size();

    VkBuffer stagingBuffer;
    VmaAllocation stagingBufferMemory;
    commandPool.createBuffer2(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    //vkMapMemory(ctx.device->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    vmaMapMemory(ctx.allocator, stagingBufferMemory, &data);
    memcpy(data, indicies.data(), (size_t) bufferSize);
    //vkUnmapMemory(ctx.device->getDevice(), stagingBufferMemory);
    vmaUnmapMemory(ctx.allocator, stagingBufferMemory);

    commandPool.createBuffer2(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    commandPool.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(ctx.device->getDevice(), stagingBuffer, nullptr);
    //vkFreeMemory(ctx.device->getDevice(), stagingBufferMemory, nullptr);
    vmaFreeMemory(ctx.allocator, stagingBufferMemory);
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
