#pragma once

#include <map>
#include <memory>
#include <vector>

#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "context.hpp"
#include "devices/window.hpp"
#include "scenes/game_object.hpp"
#include "geometry/shapes/square.hpp"
#include "instance.hpp"
#include "logical_device.hpp"
#include "material.hpp"
#include "physical_device.hpp"
#include "pipeline.hpp"
#include "render_pass.hpp"
#include "shader.hpp"
#include "surface.hpp"
#include "swap_chain.hpp"


class VulkanGraphics {

public:
    VulkanGraphics(Window &window);
    ~VulkanGraphics();

    GameObject* addGameObject();

    std::unique_ptr<GraphicsContext> ctx;
    std::unique_ptr<CommandPool> commandPool;
    std::unique_ptr<SwapChain> swapChain;

    void update();

private:
    Window &window;


    std::unique_ptr<Material> material;

    std::vector<GameObject*> gameObjects;

    std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;

    void createSyncObjects();
    void cleanupSyncObjects();
    void onResize();
};
