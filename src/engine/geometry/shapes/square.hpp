#pragma once

#include <vector>

#include "geometry/mesh.hpp"


class Square : public Mesh
{
public:
    Square(LogicalDevice *logicalDevice) : Mesh(
        logicalDevice,
        std::vector<Vertex>{
            {{-0.5f, -0.5f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f}}
        },
        std::vector<uint16_t>{
            0, 1, 2, 2, 3, 0
        }
    ) {}
};
