
#include "ant.hpp"


AntObject::AntObject()
    : GameObject() { }


AntObject::AntObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : GameObject(pos, size, sprite, color, velocity) { }


void AntObject::addHeldItem(GameObject *item)
{
    this->heldItems.push_back(item);
}

