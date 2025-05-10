#pragma once

#include "logical_device.hpp"


class RenderPass
{
public:
    RenderPass(Device &device, VkFormat &imageFormat);
    ~RenderPass();

    VkRenderPass const getRenderPass() const { return renderPass; }

private:
    VkRenderPass renderPass;
    VkFormat imageFormat;

    Device &device;

    void createRenderPass();
};
