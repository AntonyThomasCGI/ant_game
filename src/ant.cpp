
#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>

#define GLM_ENABLE_EXPERIMENTAL


#include "ant.hpp"
#include <glm/gtx/matrix_decompose.hpp>


const int LEG_COUNT = 6;
const bool DEBUG = false;


AntGameObject::AntGameObject(Engine *engine)
    : GameObject()
{
    // Transform component
    std::shared_ptr<TransformComponent> tc = std::make_shared<TransformComponent>();
    tc->scale = glm::vec2(100.0f);

    addComponent(tc);

    // Mesh component
    std::shared_ptr<MeshComponent> mc = std::make_shared<MeshComponent>();
    std::shared_ptr<Mesh> square = std::make_shared<Square>(*engine->graphics->ctx);
    square->createBuffers(*engine->graphics->commandPool);
    mc->mesh = square;

    addComponent(mc);

    // Material component
    std::shared_ptr<Material> material = std::make_shared<Material>(
        *engine->graphics->ctx, *engine->graphics->commandPool, *engine->graphics->swapChain);

    material->setTexturePath("resources/textures/ant1_no_leg.png");
    material->setShader(
        *engine->graphics->swapChain, "resources/shaders/flat_vert.spv", "resources/shaders/flat_frag.spv");

    std::shared_ptr<MaterialComponent> materialComponent = std::make_shared<MaterialComponent>();
    materialComponent->material = material;

    addComponent(materialComponent);

    // Material component
    std::shared_ptr<Material> legMat = std::make_shared<Material>(
        *engine->graphics->ctx, *engine->graphics->commandPool, *engine->graphics->swapChain);

    legMat->setTexturePath("resources/textures/ant1_leg.png");
    legMat->setShader(
        *engine->graphics->swapChain, "resources/shaders/flat_vert.spv", "resources/shaders/flat_frag.spv");

    legMaterialComponent = std::make_shared<MaterialComponent>();
    legMaterialComponent->material = legMat;

    for (uint16_t i = 0; i < LEG_COUNT; i++) {
        auto leg = createLeg(engine);
        legs[i] = leg;

        LegState state;
        switch (i) {
            case 0: state.translateOffset = glm::vec2(0.06f, -0.15f);  state.rotateOffset = 90.0f;  state.scaleOffset = glm::vec2(0.4, 0.4);  state.target = glm::vec2(30.0f, 30.0f); state.isLeft = false; break;
            case 1: state.translateOffset = glm::vec2(-0.06f, -0.15f); state.rotateOffset = -90.0f; state.scaleOffset = glm::vec2(-0.4, 0.4); state.target = glm::vec2(-30.0f, 5.0f); break;
            case 2: state.translateOffset = glm::vec2(0.08f, -0.22f);   state.rotateOffset = 90.0f;  state.scaleOffset = glm::vec2(0.4, 0.4);  state.target = glm::vec2(30.0f, -5.0f); state.isLeft = false; break;
            case 3: state.translateOffset = glm::vec2(-0.08f, -0.22f);  state.rotateOffset = -90.0f; state.scaleOffset = glm::vec2(-0.4, 0.4); state.target = glm::vec2(-30.0f, 15.0f); break;
            case 4: state.translateOffset = glm::vec2(0.04f, -0.3f);  state.rotateOffset = 90.0f;  state.scaleOffset = glm::vec2(0.4, 0.4);  state.target = glm::vec2(30.0f, -15.0f); state.isLeft = false; break;
            case 5: state.translateOffset = glm::vec2(-0.04f, -0.3f); state.rotateOffset = -90.0f; state.scaleOffset = glm::vec2(-0.4, 0.4); state.target = glm::vec2(-30.0f, -30.0f); break;
        }
        legStates[i] = state;

        auto tc = leg->getComponent<TransformComponent>();
        tc->translate = state.translateOffset;
        tc->rotate = state.rotateOffset;
        tc->scale = state.scaleOffset;
        tc->rotatePivot = glm::vec2(0.0f, 0.2f);
    }

    if ( DEBUG ) {
        for (uint16_t i = 0; i < LEG_COUNT; i++) {
            std::shared_ptr<Locator> locator = std::make_unique<Locator>(engine, legStates[i].target.x, legStates[i].target.y);
            engine->graphics->addGameObject(locator);
            locators[i] = locator;
        }
    }


    // Create six leg GameObjects
    // addChildren / however parent/child heirarchy will work
    // add updateLegs() method to animate.
}

std::shared_ptr<GameObject> AntGameObject::createLeg(Engine *engine)
{
    std::shared_ptr<GameObject> leg = std::make_shared<GameObject>();
    engine->graphics->addGameObject(leg);

    // Transform component
    std::shared_ptr<TransformComponent> tc = std::make_shared<TransformComponent>();
    tc->parentObject = this->getComponent<TransformComponent>().get();

    leg->addComponent(tc);

    // Mesh component
    std::shared_ptr<MeshComponent> mc = std::make_shared<MeshComponent>();
    std::shared_ptr<Mesh> square = std::make_shared<Square>(*engine->graphics->ctx);
    square->createBuffers(*engine->graphics->commandPool);
    mc->mesh = square;

    leg->addComponent(mc);
    leg->addComponent(legMaterialComponent);

    return leg;
}


void AntGameObject::update(float deltaTime)
{
    //return;
    // Calculate basic IK for legs.
    for (uint16_t i = 0; i < LEG_COUNT; i++) {
        std::shared_ptr<GameObject> leg = legs[i];
        auto legTC = leg->getComponent<TransformComponent>();

        glm::vec2 worldTranslate = legTC->parentObject->translate + legTC->translate;
        glm::vec2 toTargetVec = legStates[i].target - worldTranslate;

        //std::cout << toTargetVec.x <<  ", " << toTargetVec.y << std::endl;

        float distanceToTarget = glm::length(toTargetVec);

        //std::cout << distanceToTarget << std::endl;

        //std::cout << distanceToTarget << std::endl;
        if (distanceToTarget > 50.0f) {
            // Reposition target if it is too far away.
            // TODO: walking backwards? dot?
            // TODO: maybe only 1 leg each side should have a target, then other two
            //       legs are always offset with particular ant-like cadence.

            glm::mat4 antTransform = legTC->parentObject->getTransform();

            std::cout << legStates[i].isLeft << std::endl;

            glm::vec3 newTargetVec = legStates[i].isLeft ? glm::vec3(-0.3f, 0.4f, 0.0) : glm::vec3(0.3f, 0.4f, 0.0);
            glm::mat4 targetTransform{1.0f};
            targetTransform = glm::translate(targetTransform, newTargetVec);

            glm::mat4 inverseScale{1.0f};
            inverseScale = glm::scale(inverseScale, -glm::vec3(legTC->parentObject->scale, 1.0f));

            //glm::mat4 worldTargetTransform = targetTransform * antTransform;
            glm::mat4 worldTargetTransform = antTransform * targetTransform * inverseScale;

            glm::vec2 newWorldPosition = worldTargetTransform[3];

            legStates[i].target = newWorldPosition;
            std::cout << "Repositioning target to " << newWorldPosition.x << ", " << newWorldPosition.y << std::endl;

            if ( DEBUG ) {
                auto locTC = locators[i]->getComponent<TransformComponent>();
                locTC->translate = newWorldPosition;
            }

            toTargetVec = legStates[i].target - worldTranslate;
        }

        // Aim leg at target
        float toTargetRad = atan2(toTargetVec.y, toTargetVec.x);
        float toTargetDeg = toTargetRad * (180.0 / M_PI);


        float sideAdjusted = legStates[i].isLeft ? toTargetDeg -= 180.0 : toTargetDeg;

        float parentRot = legTC->parentObject->rotate;
        //float normalizedParentRot = parentRot - (360.0 * float((int)parentRot % 360));

        float rotationResult = legStates[i].rotateOffset + toTargetDeg - parentRot;

        //float clampedResult = glm::clamp(rotationResult, -65.0f + legStates[i].rotateOffset, 65.0f + legStates[i].rotateOffset);

        legTC->rotate = rotationResult;
        //legTC->rotate = glm::sin(glfwGetTime() * 5.0f) * 45.0f + state.rotateOffset;

    }
}
