
#include <string>
#include <memory>

#include "locator.hpp"
#include "components/transform.hpp"
#include "components/mesh.hpp"
#include "components/material.hpp"


const std::string locatorTexture = "resources/textures/locator.png";


Locator::Locator(Engine *engine, float x, float y)
    : GameObject()
{
    std::shared_ptr<TransformComponent> tc = std::make_shared<TransformComponent>();
    tc->scale = glm::vec2(50.0f);
    tc->translate = glm::vec2(x, y);

    addComponent(tc);

    // Mesh component
    std::shared_ptr<MeshComponent> mc = std::make_shared<MeshComponent>();
    std::shared_ptr<Mesh> square = std::make_shared<Square>(*engine->graphics->ctx);
    square->createBuffers(*engine->graphics->commandPool);
    mc->mesh = square;

    addComponent(mc);

    // Material component
    std::shared_ptr<Material> material = std::make_shared<Material>(
        *engine->graphics->ctx, *engine->graphics->commandPool, *engine->graphics->swapChain);

    material->setTexturePath(locatorTexture);
    material->setShader(
        *engine->graphics->swapChain, "resources/shaders/flat_vert.spv", "resources/shaders/flat_frag.spv");

    std::shared_ptr<MaterialComponent> materialComponent = std::make_shared<MaterialComponent>();
    materialComponent->material = material;

    addComponent(materialComponent);
};
