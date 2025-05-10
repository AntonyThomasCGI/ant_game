#pragma once

#include <vector>

#include "graphics/command_buffer.hpp"
#include "graphics/command_pool.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/swap_chain.hpp"

#include "vertex.hpp"


class Mesh
{

public:
    Mesh(Device *device, std::vector<Vertex> vertices, std::vector<uint16_t> indicies = {});
    ~Mesh();

    void createBuffers(CommandPool &commandPool);
    void draw(CommandBuffer &commandBuffer, SwapChain &swapChain);

private:
    Device *device;
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indicies;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    void createVertexBuffer(CommandPool &commandPool);
    void createIndexBuffer(CommandPool &commandPool);
};
