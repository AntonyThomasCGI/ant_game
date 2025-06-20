#pragma once

#include <glm/glm.hpp>

#include "geometry/mesh.hpp"


class SpriteRenderer
{
public:
    SpriteRenderer();
    ~SpriteRenderer();

    void draw(glm::mat4 transform);

private:
    //Shader shader;
    //Mesh mesh;
};
