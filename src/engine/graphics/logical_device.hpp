#pragma once


#include "physical_device.hpp"
#include "surface.hpp"


class CommandPool;


class LogicalDevice
{
public:
    LogicalDevice(Instance &instance, Surface &surface, PhysicalDevice &physicalDevice);
    ~LogicalDevice();

    const VkDevice &getDevice() const { return device; }
    VkQueue &getGraphicsQueue() { return graphicsQueue; }
    VkQueue &getPresentQueue() { return presentQueue; }
    PhysicalDevice &getPhysicalDevice() { return physicalDevice; }

private:
    VkDevice device;
    Instance &instance;
    Surface &surface;
    PhysicalDevice &physicalDevice;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    void createLogicalDevice();
};
