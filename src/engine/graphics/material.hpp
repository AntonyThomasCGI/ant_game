
#pragma once

#include <vk_mem_alloc.h>

#include <memory>

#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "shader.hpp"
#include "texture.hpp"


class Material
{
public:
    Material(GraphicsContext &ctx, CommandPool &commandPool, SwapChain &swapChain);
    //Material(std::string vertShader, std::string fragShader, std::string texturePath);
    ~Material();

    void setShader(SwapChain &swapChain, std::string vertShader, std::string fragShader);
    void setTexturePath(std::string texturePath);

    void bindPipeline(CommandBuffer &commandBuffer, uint32_t currentFrame);
    void bindDescriptorSetsWithOffset(CommandBuffer & commandBuffer, uint32_t currentFrame, uint32_t meshN);

    void updateUniformBuffer(glm::mat4 transform, glm::vec3 color, uint32_t currentFrame, size_t meshN);

    void cleanupDescriptorPool();
    void createDescriptorSets();
    void createDescriptorPool();

    const size_t getMaxMeshCount() { return maxMeshCount; }
    void setMaxMeshCount(size_t newValue);

    const std::string getCurrentTexturePath() { return currentTexturePath; }


private:
    GraphicsContext &ctx;
    CommandPool &commandPool;
    SwapChain &swapChain;

    std::string currentTexturePath;

    GraphicsPipeline *graphicsPipeline;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<TextureImage> textureImage;

    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkBuffer> uniformBuffers;
    //std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<VmaAllocation> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;

    void createUniformBuffers();
    void createDescriptorSetLayout();

    size_t maxMeshCount = 10;
    VkDeviceSize alignedUboSize;
};
