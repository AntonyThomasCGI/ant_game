
#include "game_object.hpp"

//TODO, resource manager
# ifdef /* bad */ __APPLE__
    const std::string vertPath = "/Users/antony/dev/vulkan_grid/shaders/flat_vert.spv";
    const std::string fragPath = "/Users/antony/dev/vulkan_grid/shaders/flat_frag.spv";
# else
    const std::string vertPath = "C:\\Users\\GGPC\\dev\\vulkan_grid\\shaders\\flat_vert.spv";
    const std::string fragPath = "C:\\Users\\GGPC\\dev\\vulkan_grid\\shaders\\flat_frag.spv";
# endif



GameObject::GameObject(LogicalDevice &logicalDevice, CommandPool &commandPool, SwapChain &swapChain)
{
    shader = std::make_unique<Shader>(logicalDevice.getDevice(), commandPool, swapChain, vertPath, fragPath);

    mesh = std::make_unique<Square>(&logicalDevice);
    mesh->createBuffers(commandPool);
}


GameObject::~GameObject()
{

}


void GameObject::move(glm::vec2 trans, float rot)
{
    translate += trans;
    rotate += rot;
}


void GameObject::draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame)
{
    shader->bind(commandBuffer.getCommandBuffer(), currentFrame);

    mesh->draw(commandBuffer, swapChain);

    glm::mat4 transform{1.0f};
    //ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = glm::translate(transform, glm::vec3(translate, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, 1.0f));

    shader->updateUniformBuffer(transform, currentFrame, swapChain);
}
