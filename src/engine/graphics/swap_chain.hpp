#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "devices/window.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "render_pass.hpp"
#include "surface.hpp"


class SwapChain
{
public:
    SwapChain(Surface &surface, PhysicalDevice &physicalDevice, Device &device, Window &window);
    ~SwapChain();

    void cleanupSwapChain();

    const VkSwapchainKHR &getSwapChain() const { return swapChain; }

    const VkRenderPass getRenderPass() const { return renderPass->getRenderPass(); }

    const VkFramebuffer &getCurrentFramebuffer() const { return swapChainFramebuffers[imageIndex]; }

    const void setCurrentImageIndex(uint32_t &imageIdx) { imageIndex = imageIdx; }

    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    void createSwapChain();
    void createImageViews();
    void createFramebuffers();


private:
    VkSwapchainKHR swapChain;
    std::unique_ptr<RenderPass> renderPass;

    uint32_t imageIndex = 0;

    std::vector<VkImage> swapChainImages;

    Surface &surface;
    PhysicalDevice &physicalDevice;
    Device &device;
    Window &window;

    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};
