#pragma once

#include <array>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>


struct Vertex {
    glm::vec2 pos;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription getBindingDescription{};
        getBindingDescription.binding = 0;
        getBindingDescription.stride = sizeof(Vertex);
        getBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return getBindingDescription;
    };

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        return attributeDescriptions;
    };
};
