
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct TransformComponent
{

    TransformComponent *parentObject = nullptr;

    glm::vec2 translate = glm::vec2(0.0f, 0.0f);
    float rotate = 0.0f;
    glm::vec2 scale = glm::vec2(1.0f);
    glm::vec2 rotatePivot = glm::vec2(0.0f, 0.0f);

    void move(glm::vec2 trans, float rot)
    {
        translate += trans;
        rotate += rot;
    }

    glm::mat4 getTransform()
    {
        glm::mat4 transform{1.0f};

        transform = glm::translate(transform, glm::vec3(translate, 0.0f));

        transform = glm::translate(transform, glm::vec3(rotatePivot, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(-rotatePivot, 0.0f));

        transform = glm::scale(transform, glm::vec3(scale, 1.0f));

        //glm::mat4 transform = glm::translate(glm::mat4{1.0f}, glm::vec3(rotatePivot, 0.0f)) *
        //                      glm::scale(glm::mat4{1.0f}, glm::vec3(scale, 1.0f)) *
        //                      glm::rotate(glm::mat4{1.0f}, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)) *
        //                      glm::translate(glm::mat4{1.0f}, -glm::vec3(rotatePivot, 0.0f)) *
        //                      glm::translate(glm::mat4{1.0f}, glm::vec3(translate, 0.0f));

        if (parentObject != nullptr) {
            transform = parentObject->getTransform() * transform;
        }

        return transform;
    }



};
