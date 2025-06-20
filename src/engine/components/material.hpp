
#pragma once

#include <memory>

#include "graphics/material.hpp"


struct MaterialComponent
{
    std::shared_ptr<Material> material;
};
