#pragma once

#include <memory>

#include "geometry/mesh.hpp"


struct MeshComponent {
    std::shared_ptr<Mesh> mesh;
};
