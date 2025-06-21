#pragma once

#include <vector>

#include <vk_mem_alloc.h>

#include <glm/glm.hpp>

#include "graphics/command_buffer.hpp"
#include "graphics/command_pool.hpp"
#include "graphics/context.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/swap_chain.hpp"

#include "vertex.hpp"


class Mesh
{

public:
    Mesh(GraphicsContext &ctx, std::vector<Vertex> vertices, std::vector<uint16_t> indicies = {});
    ~Mesh();

    void createBuffers(CommandPool &commandPool);
    void draw(CommandBuffer &commandBuffer, SwapChain &swapChain);

private:
    GraphicsContext &ctx;
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indicies;

    VkBuffer vertexBuffer;
    VmaAllocation vertexBufferMemory;
    VkBuffer indexBuffer;
    VmaAllocation indexBufferMemory;

    void createVertexBuffer(CommandPool &commandPool);
    void createIndexBuffer(CommandPool &commandPool);
};
