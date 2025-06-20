
#include <string>
#include "game_object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


const std::string vertPath = "resources/shaders/flat_vert.spv";
const std::string fragPath = "resources/shaders/flat_frag.spv";
const std::string defaultSprite = "resources/textures/ant1.png";


GameObject::GameObject(GraphicsContext &ctx, CommandPool &commandPool, SwapChain &swapChain)
    : ctx(ctx)
{
    //material = new Material(ctx, commandPool);
    //material->setTexturePath(defaultSprite);
    //material->setShader(swapChain, vertPath, fragPath);

    mesh = std::make_unique<Square>(ctx);
    mesh->createBuffers(commandPool);
}


GameObject::~GameObject()
{
}


void GameObject::setMaterial(Material *mat)
{
    material = mat;
}


void GameObject::setSpritePath(std::string texturePath)
{
    // TODO, conditionally need to cleanup i think
    //material->cleanupDescriptorPool();
    material->setTexturePath(texturePath);
    material->createDescriptorPool();
    material->createDescriptorSets();

}


void GameObject::draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame, size_t meshN = 0)
{
    material->bindDescriptorSetsWithOffset(commandBuffer, currentFrame, meshN);

    std::shared_ptr<TransformComponent> tc = getComponent<TransformComponent>();
    glm::mat4 transform = tc->getTransform();

    material->updateUniformBuffer(transform, color, currentFrame, meshN);

    mesh->draw(commandBuffer, swapChain);
}
