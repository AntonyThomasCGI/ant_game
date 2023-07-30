
#include "ant.hpp"


AntObject::AntObject()
    : GameObject() { }


AntObject::AntObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : GameObject(pos, size, sprite, color, velocity)
{
    this->initialPosition = pos;
    this->previousPosition = pos;
    this->previousRotation = 0.0;
}


void AntObject::addHeldItem(GameObject *item)
{
    this->heldItems.push_back(item);
}


void AntObject::move(glm::vec2 translate, float rotate)
{
    this->previousPosition = this->Position;
    this->previousRotation = this->Rotation;
    this->Position += translate;
    this->Rotation += rotate;
};


void getHitBox()
{

}
