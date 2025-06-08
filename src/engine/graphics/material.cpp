
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vk_enum_string_helper.h>

#include <array>

#include "constants.hpp"
#include "material.hpp"
#include "resource_manager.hpp"


struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 color;
};


Material::Material(GraphicsContext &ctx, CommandPool &commandPool)
    : ctx(ctx), commandPool(commandPool)
{ }


Material::~Material()
{
    delete graphicsPipeline;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        //vkDestroyBuffer(device.getDevice(), uniformBuffers[i], nullptr);
        //vkFreeMemory(device.getDevice(), uniformBuffersMemory[i], nullptr);
        vmaUnmapMemory(ctx.allocator, uniformBuffersMemory[i]);
        vmaDestroyBuffer(ctx.allocator, uniformBuffers[i], uniformBuffersMemory[i]);
    }
    vkDestroyDescriptorPool(ctx.device->getDevice(), descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(ctx.device->getDevice(), descriptorSetLayout, nullptr);
}


void Material::cleanupDescriptorPool()
{
    vkDestroyDescriptorPool(ctx.device->getDevice(), descriptorPool, nullptr);
}


void Material::bind(CommandBuffer &commandBuffer, uint32_t currentFrame)
{
    vkCmdBindPipeline(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getGraphicsPipeline());

    vkCmdBindDescriptorSets(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipelineLayout(), 0, 1, &descriptorSets[currentFrame], 0, nullptr);
}


void Material::bindDescriptorSetsWithOffset(CommandBuffer & commandBuffer, uint32_t currentFrame, uint32_t meshN)
{
    VkDeviceSize offset = meshN * alignedUboSize;
    vkCmdBindDescriptorSets(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipelineLayout(), 0, 1, &descriptorSets[currentFrame], 1, (uint32_t*)&offset);
}

void Material::setMaxMeshCount(size_t newValue)
{
    maxMeshCount = newValue;

    vkDestroyShaderModule(ctx.device->getDevice(), shader->vertShaderModule, nullptr);
    vkDestroyShaderModule(ctx.device->getDevice(), shader->fragShaderModule, nullptr);

    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
}


void Material::updateUniformBuffer(glm::mat4 transform, glm::vec3 color, uint32_t currentFrame, SwapChain &swapChain, size_t meshN)
{
    UniformBufferObject ubo{};

    ubo.model = transform;

    //ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //ubo.proj = glm::perspective(glm::radians(45.0f), swapChain.swapChainExtent.width / (float) swapChain->swapChainExtent.height, 0.1f, 10.0f);

    //ubo.model = glm::mat4(1.0f);
    //ubo.view = glm::ortho(0.0f, (float) swapChain->swapChainExtent.width, (float) swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
    //float halfWidth = (float) swapChain->swapChainExtent.width / 2.0f;
    //float halfHeight = (float) swapChain->swapChainExtent.height / 2.0f;
    //ubo.view = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
    //ubo.view = glm::perspective(glm::radians(45.0f), swapChain->swapChainExtent.width / (float) swapChain->swapChainExtent.height, 0.1f, 10.0f);
    //ubo.view = glm::mat4(1.0f);
    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    float aspectRatio = (float) swapChain.swapChainExtent.width / (float) swapChain.swapChainExtent.height;
    //float targetWidth =  (float) swapChain->swapChainExtent.width;
    //float targetHeight = (float) swapChain->swapChainExtent.height;
    // TODO, get the values from app startup or something
    float targetWidth = 800.0f;
    float targetHeight = 600.0f;
    float targetAspect = targetWidth / targetHeight;

    ubo.proj = glm::ortho(-(targetWidth / 2.0f), (targetWidth / 2.0f), -(targetHeight / 2.0f), (targetHeight / 2.0f), -100.0f, 100.0f);

    //ubo.view = glm::scale(glm::mat4(1.0f), glm::vec3((float) swapChain->swapChainExtent.width / (float) swapChain->swapChainExtent.height));

    ubo.proj[1][1] *= -1;  // Flip y-axis (open-gl oriented library).

    ubo.color = color;

    memcpy((char*)uniformBuffersMapped[currentFrame] + meshN * alignedUboSize, &ubo, sizeof(ubo));
}


void Material::setShader(SwapChain &swapChain, std::string vertShader, std::string fragShader)
{

    createDescriptorSetLayout();

    shader = std::make_unique<Shader>(*ctx.device, vertShader, fragShader);

    graphicsPipeline = new GraphicsPipeline(*ctx.device, swapChain, descriptorSetLayout, shader->vertShaderModule, shader->fragShaderModule);

    vkDestroyShaderModule(ctx.device->getDevice(), shader->vertShaderModule, nullptr);
    vkDestroyShaderModule(ctx.device->getDevice(), shader->fragShaderModule, nullptr);

    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
}


void Material::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    //uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    uboLayoutBinding.descriptorCount = 1;

    // --The descriptor is only referenced in the vertex shader--
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;  // optional

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(ctx.device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}


void Material::createDescriptorPool()
{
    //if (descriptorPool != VK_NULL_HANDLE) {

    //    vkDestroyDescriptorPool(device.getDevice(), descriptorPool, nullptr);
    //};

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(ctx.device->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}


void Material::createDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    VkResult result = vkAllocateDescriptorSets(ctx.device->getDevice(), &allocInfo, descriptorSets.data());

    if (result != VK_SUCCESS) {
        std::cout << "Got result: " << string_VkResult(result) << std::endl;;
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImage->getTextureImageView();
        imageInfo.sampler = textureImage->getTextureSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        //descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        //descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(ctx.device->getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

}


void Material::createUniformBuffers()
{
    VkPhysicalDeviceProperties properties{};
    // Could store these props on physical device at program start instead.
    vkGetPhysicalDeviceProperties(ctx.physicalDevice->getPhysicalDevice(), &properties);
    VkDeviceSize minAlignment = properties.limits.minUniformBufferOffsetAlignment;

    VkDeviceSize uboSize = sizeof(UniformBufferObject);

    auto alignedSize = [](size_t value, size_t alignment) {
        return (value + alignment - 1) & ~(alignment - 1);
    };

    alignedUboSize = alignedSize(uboSize, minAlignment);
    VkDeviceSize bufferSize = alignedUboSize * maxMeshCount;

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        commandPool.createBuffer2(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        //commandPool.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vmaMapMemory(ctx.allocator, uniformBuffersMemory[i], &uniformBuffersMapped[i]);
        //vkMapMemory(device.getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
        //vkMapMemory(device.getDevice(), uniformBuffersMemory[i].deviceMemory, uniformBuffersMemory[i].offset, bufferSize, 0, &uniformBuffersMapped[i]);
        //memcpy(uniformBuffersMapped[i], &uniformBuffers[i], bufferSize);
    }
}


void Material::setTexturePath(std::string texturePath)
{

    currentTexturePath = texturePath;
    //textureImage = ResourceManager::getTexture(texturePath);
    textureImage = std::make_unique<TextureImage>(ctx, commandPool, texturePath);
}
