
#include <chrono>
#include <iostream>
#include <memory>

#include <vulkan/vk_enum_string_helper.h>

#include "constants.hpp"
#include "graphics.hpp"



VulkanGraphics::VulkanGraphics(Window &window) : window(window)
{
    instance = std::make_unique<Instance>(window);
    surface = std::make_unique<Surface>(*instance, window);
    physicalDevice = std::make_unique<PhysicalDevice>(*instance, *surface);
    physicalDevice->pickPhysicalDevice();
    device = std::make_unique<Device>(*instance, *surface, *physicalDevice);

    // Create the allocator
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = physicalDevice->getPhysicalDevice();

    allocatorCreateInfo.device = device->getDevice();
    allocatorCreateInfo.instance = instance->getInstance();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    // End creating allocator

    swapChain = std::make_unique<SwapChain>(*surface, *physicalDevice, *device, window);
    commandPool = std::make_unique<CommandPool>(allocator, *physicalDevice, *device, *surface);

    // hmm, maybe it's ok to use one set of cmd buffers / sync objects for every asset?
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        commandBuffers[i] = std::make_unique<CommandBuffer>(*device, *commandPool);
    }

    createSyncObjects();
}


VulkanGraphics::~VulkanGraphics()
{
    vkDeviceWaitIdle(device->getDevice());

    for (auto const& [name, gameObj] : gameObjects) {
        delete gameObj;
    }
    cleanupSyncObjects();
    vmaDestroyAllocator(allocator);
}


GameObject* VulkanGraphics::addGameObject(std::string name)
{
    GameObject *gameObj = new GameObject(*physicalDevice, *device, *commandPool, *swapChain);
    gameObjects[name] = gameObj;

    return gameObj;
}


void VulkanGraphics::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
    }
}


void VulkanGraphics::cleanupSyncObjects() {
    vkDeviceWaitIdle(device->getDevice());

    for (size_t i = 0; i < imageAvailableSemaphores.size(); i++) {
        vkDestroySemaphore(device->getDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(device->getDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(device->getDevice(), inFlightFences[i], nullptr);
    }
}

void VulkanGraphics::onResize() {
    int width = 0, height = 0;
    window.getFramebufferSize(width, height);
    // If window is minimized, pause until it comes back.
    while (width == 0 || height == 0) {
        window.getFramebufferSize(width, height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device->getDevice());

    cleanupSyncObjects();
    swapChain->cleanupSwapChain();

    swapChain->createSwapChain();
    swapChain->createImageViews();
    swapChain->createFramebuffers();
    createSyncObjects();
}

void VulkanGraphics::update()
{
    vkWaitForFences(device->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device->getDevice(), swapChain->getSwapChain(), 5e+9, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        onResize();
        return;
    } else if (result != VK_SUCCESS) {
        std::cerr << string_VkResult(result) << std::endl;
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    swapChain->setCurrentImageIndex(imageIndex);

    // Only reset the fence if we are submitting work.
    vkResetFences(device->getDevice(), 1, &inFlightFences[currentFrame]);

    commandBuffers[currentFrame]->start();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain->getRenderPass();
    renderPassInfo.framebuffer = swapChain->getCurrentFramebuffer();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain->swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[currentFrame]->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    for (const auto& [gameObjName, gameObj] : gameObjects) {
        gameObj->draw(*commandBuffers[currentFrame].get(), *swapChain, currentFrame);
    }

    vkCmdEndRenderPass(commandBuffers[currentFrame]->getCommandBuffer());

    commandBuffers[currentFrame]->end();


    commandBuffers[currentFrame]->submit(imageAvailableSemaphores[currentFrame], renderFinishedSemaphores[currentFrame], inFlightFences[currentFrame]);

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain->getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);
    //**

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
