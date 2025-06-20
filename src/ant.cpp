
#include <iostream>

#include "ant.hpp"


AntGameObject::AntGameObject()
    : GameObject()
{
    std::cout << "Create ant" << std::endl;

    // Create six leg GameObjects
    // addChildren / however parent/child heirarchy will work
    // add updateLegs() method to animate.
}


