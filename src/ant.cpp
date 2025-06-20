
#include <iostream>

#include "ant.hpp"


AntGameObject::AntGameObject(GraphicsContext &ctx, CommandPool &commandPool, SwapChain &swapChain)
    : GameObject(ctx, commandPool, swapChain)
{
    std::cout << "Create ant" << std::endl;

    // Create six leg GameObjects
    // addChildren / however parent/child heirarchy will work
    // add updateLegs() method to animate.
}


