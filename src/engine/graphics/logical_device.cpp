
#include <set>
#include <stdexcept>

#include "logical_device.hpp"


Device::Device(Instance &instance, Surface &surface, PhysicalDevice &physicalDevice) : instance(instance), surface(surface), physicalDevice(physicalDevice)
{
    createDevice();
}

Device::~Device()
{

    vkDestroyDevice(device, nullptr);
}

void Device::createDevice() {
    QueueFamilyIndices indicies = surface.findQueueFamilies(physicalDevice.getPhysicalDevice());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indicies.graphicsFamily.value(),
        indicies.presentFamily.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indicies.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice.deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = physicalDevice.deviceExtensions.data();

    if (instance.enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(instance.validationLayers.size());
        createInfo.ppEnabledLayerNames = instance.validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice.getPhysicalDevice(), &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indicies.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indicies.presentFamily.value(), 0, &presentQueue);
}

