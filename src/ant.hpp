
#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "engine/components/material.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"
#include "engine/engine.hpp"
#include "engine/scenes/game_object.hpp"
#include "engine/scenes/locator.hpp"


class AntGameObject : public GameObject
{

public:
    AntGameObject(Engine *engine);

    struct LegState {
        // Static offsets for defining where leg is placed.
        glm::vec2 translateOffset;
        float rotateOffset;
        glm::vec2 scaleOffset;

        glm::vec2 target;  // World space target position (point leg at).
    };

    std::shared_ptr<GameObject> createLeg(Engine *engine);

    void update(float deltaTime);

private:

    std::shared_ptr<MaterialComponent> legMaterialComponent;

    std::shared_ptr<GameObject> legs[6];
    LegState legStates[6];


    // Debug stuff
    std::shared_ptr<Locator> locators[6];
};
