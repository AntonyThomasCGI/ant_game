
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <rocket.hpp>

#include <iostream>

#include "engine.hpp"


Engine::Engine(Window &window) : window(window)
{
    graphics = std::make_unique<VulkanGraphics>(window);
}

Engine::~Engine()
{
}

void Engine::update()
{
    graphics->update();
}
