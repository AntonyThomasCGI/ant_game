#pragma once


#include "physical_device.hpp"
#include "surface.hpp"


class CommandPool;


class Device
{
public:
    Device(Instance &instance, Surface &surface, PhysicalDevice &physicalDevice);
    ~Device();

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

    void createDevice();
};
