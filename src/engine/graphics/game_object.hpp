#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "command_buffer.hpp"
#include "command_pool.hpp"
#include "context.hpp"
#include "geometry/shapes/square.hpp"
#include "material.hpp"
#include "physical_device.hpp"
#include "swap_chain.hpp"



class GameObject
{
public:

    GameObject(GraphicsContext &ctx, CommandPool &commandPool, SwapChain &swapChain);
    ~GameObject();

    void move(glm::vec2 trans, float rot);

    void setMaterial(Material *mat);

    void setSpritePath(std::string texturePath);

    GameObject *parentObject = nullptr;

    glm::mat4 getTransform();

    glm::vec2 baseSize = glm::vec2(100.0f);
    glm::vec2 translate = glm::vec2(0.0f, 0.0f);
    float rotate = 0.0f;
    glm::vec2 scale = glm::vec2(1.0f);
    glm::vec2 rotatePivot = glm::vec2(0.0f, 0.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    void draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame, size_t meshN);

    Material *material;

private:
    GraphicsContext &ctx;

    std::unique_ptr<Square> mesh;
};
