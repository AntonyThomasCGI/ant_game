
#pragma once

#include <memory>

#include "graphics/material.hpp"


struct MaterialComponent
{
    std::unique_ptr<Material> material;
}