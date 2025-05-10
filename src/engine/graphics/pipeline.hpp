#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.hpp"
#include "render_pass.hpp"
#include "swap_chain.hpp"


class GraphicsPipeline
{
public: GraphicsPipeline(LogicalDevice &logicalDevice, SwapChain &swapChain, VkDescriptorSetLayout &descriptorSet, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);
    ~GraphicsPipeline();

    VkPipeline getGraphicsPipeline() const { return graphicsPipeline; };

    VkPipelineLayout getPipelineLayout() const { return pipelineLayout; };

private:
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    LogicalDevice &logicalDevice;

    void createGraphicsPipeline(SwapChain &swapChain, VkDescriptorSetLayout &descriptorSet, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);
};
