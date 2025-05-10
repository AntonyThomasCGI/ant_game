#pragma once

#include <memory>

#include "devices/window.hpp"
#include "instance.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "surface.hpp"

class GraphicsContext
{
    public:
        GraphicsContext(Window &window);
        ~GraphicsContext();
        
        std::unique_ptr<Instance> instance;
        std::unique_ptr<Surface> surface;
        std::unique_ptr<PhysicalDevice> physicalDevice;
        std::unique_ptr<Device> device;

    private:
        Window &window;
};
