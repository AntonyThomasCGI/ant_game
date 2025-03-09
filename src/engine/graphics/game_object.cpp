
#include <string>
#include "game_object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


const std::string vertPath = "resources/shaders/flat_vert.spv";
const std::string fragPath = "resources/shaders/flat_frag.spv";
const std::string defaultSprite = "resources/textures/ant1.png";


GameObject::GameObject(PhysicalDevice &physicalDevice, LogicalDevice &logicalDevice, CommandPool &commandPool, SwapChain &swapChain)
    : physicalDevice(physicalDevice), commandPool(commandPool)
{
    material = new Material(logicalDevice);
    material->setTexturePath(physicalDevice, commandPool, defaultSprite);
    material->setShader(commandPool, swapChain, vertPath, fragPath);

    mesh = std::make_unique<Square>(&logicalDevice);
    mesh->createBuffers(commandPool);
}


GameObject::~GameObject()
{
    delete material;
}


void GameObject::setSpritePath(std::string texturePath)
{
    // TODO
    material->cleanupDescriptorPool();
    material->setTexturePath(physicalDevice, commandPool, texturePath);
    material->createDescriptorPool();
    material->createDescriptorSets();

}


void GameObject::move(glm::vec2 trans, float rot)
{
    translate += trans;
    rotate += rot;
}


void GameObject::draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame)
{
    material->bind(commandBuffer, currentFrame);

    glm::mat4 transform{1.0f};
    //ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = glm::translate(transform, glm::vec3(translate, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, 1.0f));

    material->updateUniformBuffer(transform, color, currentFrame, swapChain);

    mesh->draw(commandBuffer, swapChain);
}
