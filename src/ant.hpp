#ifndef ANT_HPP
#define ANT_HPP


#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.hpp"

#include <vector>


class AntObject : public GameObject
{
public:
    std::vector<GameObject*> heldItems;
    glm::vec2 initialPosition;
    glm::vec2 previousPosition;
    float previousRotation;

    void addHeldItem(GameObject *item);

    void move(glm::vec2 translate, float rotate);

    AntObject();
    AntObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
};

#endif
