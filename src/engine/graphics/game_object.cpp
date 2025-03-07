
#include <string>
#include "game_object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//TODO, resource manager
# ifdef /* bad */ __APPLE__
    const std::string vertPath = "/Users/antony/dev/vulkan_grid/shaders/flat_vert.spv";
    const std::string fragPath = "/Users/antony/dev/vulkan_grid/shaders/flat_frag.spv";
    const std::string texturePath = "/Users/antony/dev/vulkan_grid/textures/ant.png";
# else
    const std::string vertPath = "C:\\Users\\GGPC\\dev\\vulkan_grid\\shaders\\flat_vert.spv";
    const std::string fragPath = "C:\\Users\\GGPC\\dev\\vulkan_grid\\shaders\\flat_frag.spv";
    const std::string texturePath = "C:\\Users\\GGPC\\dev\\vulkan_grid\\textures\\ant.png";
# endif




GameObject::GameObject(PhysicalDevice &physicalDevice, LogicalDevice &logicalDevice, CommandPool &commandPool, SwapChain &swapChain)
{
    material = new Material(logicalDevice);
    material->setTexturePath(physicalDevice, commandPool, texturePath);
    material->setShader(commandPool, swapChain, vertPath, fragPath);

    mesh = std::make_unique<Square>(&logicalDevice);
    mesh->createBuffers(commandPool);
}


GameObject::~GameObject()
{
    delete material;
}


void GameObject::move(glm::vec2 trans, float rot)
{
    translate += trans;
    rotate += rot;
}


void GameObject::draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame)
{
    material->bind(commandBuffer, currentFrame);

    std::cout << "bind mesh" << std::endl;

    std::cout << "here" << std::endl;
    glm::mat4 transform{1.0f};
    //ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = glm::translate(transform, glm::vec3(translate, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, 1.0f));

    material->updateUniformBuffer(transform, color, currentFrame, swapChain);

    mesh->draw(commandBuffer, swapChain);
}
