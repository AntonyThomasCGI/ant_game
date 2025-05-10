

#include "context.hpp"


GraphicsContext::GraphicsContext(Window &window)
    : window(window)
{
    instance = std::make_unique<Instance>(window);
    surface = std::make_unique<Surface>(*instance, window);
    physicalDevice = std::make_unique<PhysicalDevice>(*instance, *surface);
    physicalDevice->pickPhysicalDevice();
    device = std::make_unique<Device>(*instance, *surface, *physicalDevice);

}


GraphicsContext::~GraphicsContext()
{

}
