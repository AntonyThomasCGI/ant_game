
#include <chrono>
#include <iostream>
#include <memory>

#include <vulkan/vk_enum_string_helper.h>

#include "constants.hpp"
#include "graphics.hpp"
#include "resource_manager.hpp"

#include "components/material.hpp"



VulkanGraphics::VulkanGraphics(Window &window)
    : window(window)
{
    ctx = std::make_unique<GraphicsContext>(window);

    swapChain = std::make_unique<SwapChain>(*ctx);
    commandPool = std::make_unique<CommandPool>(*ctx);

    // hack
    //ResourceManager::createTexture(*ctx, *commandPool, "resources/textures/ant1.png");
    //ResourceManager::createTexture(*ctx, *commandPool, "resources/textures/ant2.png");
    //ResourceManager::createTexture(*ctx, *commandPool, "resources/textures/dirt.png");

    // hmm, maybe it's ok to use one set of cmd buffers / sync objects for every asset?
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        commandBuffers[i] = std::make_unique<CommandBuffer>(*ctx->device, *commandPool);
    }

    createSyncObjects();
}


VulkanGraphics::~VulkanGraphics()
{
    vkWaitForFences(ctx->device->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkDeviceWaitIdle(ctx->device->getDevice());
    cleanupSyncObjects();
}


void VulkanGraphics::addGameObject(std::shared_ptr<GameObject> obj)
{
    gameObjects.push_back(obj);
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
        if (vkCreateSemaphore(ctx->device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(ctx->device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(ctx->device->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
    }
}


void VulkanGraphics::cleanupSyncObjects() {
    vkDeviceWaitIdle(ctx->device->getDevice());

    for (size_t i = 0; i < imageAvailableSemaphores.size(); i++) {
        vkDestroySemaphore(ctx->device->getDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(ctx->device->getDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(ctx->device->getDevice(), inFlightFences[i], nullptr);
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

    vkDeviceWaitIdle(ctx->device->getDevice());

    cleanupSyncObjects();
    swapChain->cleanupSwapChain();

    swapChain->createSwapChain();
    swapChain->createImageViews();
    swapChain->createFramebuffers();
    createSyncObjects();
}

void VulkanGraphics::update()
{
    vkWaitForFences(ctx->device->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(ctx->device->getDevice(), swapChain->getSwapChain(), 5e+9, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        onResize();
        return;
    } else if (result != VK_SUCCESS) {
        std::cerr << string_VkResult(result) << std::endl;
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    swapChain->setCurrentImageIndex(imageIndex);

    // Only reset the fence if we are submitting work.
    vkResetFences(ctx->device->getDevice(), 1, &inFlightFences[currentFrame]);

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

    std::string currentTexture = "";
    size_t count = 0;
    // FIXME, sort game objects by material. Right now they just happen to be in correct order.
    for (const auto gameObj : gameObjects) {
        auto materialComp = gameObj->getComponent<MaterialComponent>();
        std::string thisTexture = materialComp->material->getCurrentTexturePath();
        if (thisTexture != currentTexture) {
            count = 0;

            materialComp->material->bindPipeline(*commandBuffers[currentFrame].get(), currentFrame);
            currentTexture = thisTexture;
        }
        gameObj->draw(*commandBuffers[currentFrame].get(), *swapChain, currentFrame, count);
        count++;
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

    vkQueuePresentKHR(ctx->device->getPresentQueue(), &presentInfo);
    //**

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
