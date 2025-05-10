
#include <array>
#include <fstream>
#include <stdexcept>

#include "constants.hpp"
#include "shader.hpp"


static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

Shader::Shader(Device &device, std::string vertShader, std::string fragShader)
    : device(device)
{
    auto vertShaderCode = readFile(vertShader);
    auto fragShaderCode = readFile(fragShader);

    vertShaderModule = createShaderModule(vertShaderCode);
    fragShaderModule = createShaderModule(fragShaderCode);
}


//Shader::Shader(VkDevice &device, CommandPool &commandPool, SwapChain &swapChain, TextureImage &textureImage, std::string vertShader, std::string fragShader)
//    : device(device)
//{
//    createDescriptorSetLayout();
//
//    auto vertShaderCode = readFile(vertShader);
//    auto fragShaderCode = readFile(fragShader);
//
//    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
//    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
//
//    graphicsPipeline = std::make_unique<GraphicsPipeline>(device, swapChain, descriptorSetLayout, vertShaderModule, fragShaderModule);
//
//    vkDestroyShaderModule(device, vertShaderModule, nullptr);
//    vkDestroyShaderModule(device, fragShaderModule, nullptr);
//
//    createUniformBuffers(commandPool);
//    createDescriptorPool();
//    createDescriptorSets(textureImage);
//}

Shader::~Shader()
{
    //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //    vkDestroyBuffer(device, uniformBuffers[i], nullptr);
    //    vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    //}
    //vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    //vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}


//void Shader::bind(VkCommandBuffer &commandBuffer, uint32_t currentFrame)
//{
//
//    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
//
//    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getGraphicsPipeline());
//}

//void Shader::updateUniformBuffer(glm::mat4 transform, glm::vec3 color, uint32_t currentFrame, SwapChain &swapChain)
//{
//    UniformBufferObject ubo{};
//
//    ubo.model = transform;
//
//    //ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    //ubo.proj = glm::perspective(glm::radians(45.0f), swapChain.swapChainExtent.width / (float) swapChain->swapChainExtent.height, 0.1f, 10.0f);
//
//    //ubo.model = glm::mat4(1.0f);
//    //ubo.view = glm::ortho(0.0f, (float) swapChain->swapChainExtent.width, (float) swapChain->swapChainExtent.height, 0.0f, -1.0f, 1.0f);
//    //float halfWidth = (float) swapChain->swapChainExtent.width / 2.0f;
//    //float halfHeight = (float) swapChain->swapChainExtent.height / 2.0f;
//    //ubo.view = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
//    //ubo.view = glm::perspective(glm::radians(45.0f), swapChain->swapChainExtent.width / (float) swapChain->swapChainExtent.height, 0.1f, 10.0f);
//    //ubo.view = glm::mat4(1.0f);
//    ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//    float aspectRatio = (float) swapChain.swapChainExtent.width / (float) swapChain.swapChainExtent.height;
//    //float targetWidth =  (float) swapChain->swapChainExtent.width;
//    //float targetHeight = (float) swapChain->swapChainExtent.height;
//    // TODO, get the values from app startup or something
//    float targetWidth = 800.0f;
//    float targetHeight = 600.0f;
//    float targetAspect = targetWidth / targetHeight;
//
//    ubo.proj = glm::ortho(-(targetWidth / 2.0f), (targetWidth / 2.0f), -(targetHeight / 2.0f), (targetHeight / 2.0f), -100.0f, 100.0f);
//
//    //ubo.view = glm::scale(glm::mat4(1.0f), glm::vec3((float) swapChain->swapChainExtent.width / (float) swapChain->swapChainExtent.height));
//
//    ubo.proj[1][1] *= -1;  // Flip y-axis (open-gl oriented library).
//
//    ubo.color = color;
//
//    memcpy(uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
//}


//void Shader::createDescriptorPool()
//{
//    std::array<VkDescriptorPoolSize, 2> poolSizes{};
//    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//    VkDescriptorPoolCreateInfo poolInfo{};
//    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//    poolInfo.pPoolSizes = poolSizes.data();
//    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//
//    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
//        throw std::runtime_error("failed to create descriptor pool!");
//    }
//}


//void Shader::createDescriptorSets(TextureImage &textureImage)
//{
//    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
//    VkDescriptorSetAllocateInfo allocInfo{};
//    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//    allocInfo.descriptorPool = descriptorPool;
//    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//    allocInfo.pSetLayouts = layouts.data();
//
//    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
//    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
//        throw std::runtime_error("failed to allocate descriptor sets!");
//    }
//
//    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//        VkDescriptorBufferInfo bufferInfo{};
//        bufferInfo.buffer = uniformBuffers[i];
//        bufferInfo.offset = 0;
//        bufferInfo.range = sizeof(UniformBufferObject);
//
//        VkDescriptorImageInfo imageInfo{};
//        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//        imageInfo.imageView = textureImage.getTextureImageView();
//        imageInfo.sampler = textureImage.getTextureSampler();
//
//        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
//        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//        descriptorWrites[0].dstSet = descriptorSets[i];
//        descriptorWrites[0].dstBinding = 0;
//        descriptorWrites[0].dstArrayElement = 0;
//        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//        descriptorWrites[0].descriptorCount = 1;
//        descriptorWrites[0].pBufferInfo = &bufferInfo;
//
//        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//        descriptorWrites[1].dstSet = descriptorSets[i];
//        descriptorWrites[1].dstBinding = 1;
//        descriptorWrites[1].dstArrayElement = 0;
//        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//        descriptorWrites[1].descriptorCount = 1;
//        descriptorWrites[1].pImageInfo = &imageInfo;
//
//        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
//    }
//}
//
//
//void Shader::createUniformBuffers(CommandPool &commandPool)
//{
//    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
//
//    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
//    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
//    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
//
//    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
//        commandPool.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
//
//        vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
//    }
//
//}



VkShaderModule Shader::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}


//void Shader::createDescriptorSetLayout()
//{
//    VkDescriptorSetLayoutBinding uboLayoutBinding{};
//    uboLayoutBinding.binding = 0;
//    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//    uboLayoutBinding.descriptorCount = 1;
//
//    // --The descriptor is only referenced in the vertex shader--
//    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//    uboLayoutBinding.pImmutableSamplers = nullptr;  // optional
//
//    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
//    samplerLayoutBinding.binding = 1;
//    samplerLayoutBinding.descriptorCount = 1;
//    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//    samplerLayoutBinding.pImmutableSamplers = nullptr;
//    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
//
//    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
//
//    VkDescriptorSetLayoutCreateInfo layoutInfo{};
//    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//    layoutInfo.pBindings = bindings.data();
//
//    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
//        throw std::runtime_error("failed to create descriptor set layout!");
//    }
//}

