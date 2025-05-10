
#include <iostream>
#include <stdexcept>

#include <vulkan/vk_enum_string_helper.h>

#include "instance.hpp"


const std::vector<const char*> Instance::validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

# ifdef __APPLE__
    std::vector<const char*> Instance::instanceExtensions = {
        "VK_KHR_portability_enumeration",
        "VK_KHR_get_physical_device_properties2"
    };
# else
    std::vector<const char*> Instance::instanceExtensions = {
        "VK_KHR_portability_enumeration"
    };
# endif


Instance::Instance(Window &window)
{
#ifdef NDEBUG
    enableValidationLayers = false;
#else
    enableValidationLayers = true;
#endif
    createInstance(window);
}


Instance::~Instance()
{
    vkDestroyInstance(instance, nullptr);
}



bool Instance::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }
    return true;
}


void Instance::createInstance(Window &window)
{
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 3, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    auto [windowExtensionCount, windowExtensions] = window.getInstanceExtensions();
    for (int i = 0; i < windowExtensionCount; i++) {
        instanceExtensions.push_back(windowExtensions[i]);
    }
    createInfo.enabledExtensionCount = instanceExtensions.size();
    createInfo.ppEnabledExtensionNames = instanceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (VkResult result = vkCreateInstance(&createInfo, nullptr, &instance); result != VK_SUCCESS) {
        std::cerr << string_VkResult(result) << std::endl;
        throw std::runtime_error("Failed to create instance!");
    }
}
