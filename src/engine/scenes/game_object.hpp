#pragma once

#include <map>
#include <memory>
#include <typeindex>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/command_buffer.hpp"
#include "graphics/command_pool.hpp"
#include "graphics/context.hpp"
#include "geometry/shapes/square.hpp"
#include "graphics/material.hpp"
#include "graphics/physical_device.hpp"
#include "graphics/swap_chain.hpp"



class GameObject
{
public:

    GameObject();
    ~GameObject();

    template <typename T>
    void addComponent(std::shared_ptr<T> component)
    {
        components[typeid(T)] = component;
    }

    template <typename T>
    std::shared_ptr<T> getComponent() {
        return std::static_pointer_cast<T>(components[typeid(T)]);
    }

    void draw(CommandBuffer &commandBuffer, SwapChain &swapChain, uint32_t currentFrame, size_t meshN);


private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> components;

};
