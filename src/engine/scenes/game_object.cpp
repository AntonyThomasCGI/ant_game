
#include <string>
#include "game_object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "components/material.hpp"
#include "components/mesh.hpp"
#include "components/transform.hpp"



const std::string vertPath = "resources/shaders/flat_vert.spv";
const std::string fragPath = "resources/shaders/flat_frag.spv";
const std::string defaultSprite = "resources/textures/ant1.png";


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}



void GameObject::draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame, size_t meshN = 0)
{
    // TODO, render should handle draw not game object.

    std::shared_ptr<MaterialComponent> matc = getComponent<MaterialComponent>();
    matc->material->bindDescriptorSetsWithOffset(commandBuffer, currentFrame, meshN);

    std::shared_ptr<TransformComponent> tc = getComponent<TransformComponent>();
    glm::mat4 transform = tc->getTransform();

    matc->material->updateUniformBuffer(transform, currentFrame, meshN);

    std::shared_ptr<MeshComponent> mesh = getComponent<MeshComponent>();
    mesh->mesh->draw(commandBuffer, swapChain);
}
