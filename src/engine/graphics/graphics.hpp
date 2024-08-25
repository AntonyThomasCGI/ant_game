#pragma once

#include <memory>
#include <vector>

#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "devices/window.hpp"
#include "geometry/shapes/square.hpp"
#include "instance.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "pipeline.hpp"
#include "render_pass.hpp"
#include "shader.hpp"
#include "surface.hpp"
#include "swap_chain.hpp"


class VulkanGraphics {

public:
    // TODO, graphics engine shouldn't require a window but oh well.
    VulkanGraphics(Window &window);
    ~VulkanGraphics();

    void createAsset();

    void update();

private:
    Window &window;
    std::unique_ptr<Instance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physicalDevice;
    std::unique_ptr<LogicalDevice> logicalDevice;
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<CommandPool> commandPool;

    std::unique_ptr<Square> square;
    std::unique_ptr<Shader> shader;
    std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;

    void createSyncObjects();
    void cleanupSyncObjects();
    void onResize();
};
