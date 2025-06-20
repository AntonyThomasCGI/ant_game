#pragma once

#include <memory>

#include "geometry/mesh.hpp"


struct MeshComponent {
    std::unique_ptr<Mesh> mesh;
};
