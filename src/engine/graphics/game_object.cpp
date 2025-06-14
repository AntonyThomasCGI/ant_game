
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


void GameObject::move(glm::vec2 trans, float rot)
{
    translate += trans;
    rotate += rot;
}

void GameObject::draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame, size_t meshN = 0)
{
    material->bindDescriptorSetsWithOffset(commandBuffer, currentFrame, meshN);

    glm::mat4 transform{1.0f};

    transform = glm::translate(transform, glm::vec3(translate, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(baseSize * scale, 1.0f));

    material->updateUniformBuffer(transform, color, currentFrame, meshN);

    mesh->draw(commandBuffer, swapChain);
}
