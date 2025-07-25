
#include <iostream>
#include <memory>
#include <random>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include "engine/app.hpp"
#include "engine/engine.hpp"
#include "engine/components/material.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"

#include "ant.hpp"



const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const unsigned int GRID_WIDTH = WIDTH / 40;
const unsigned int GRID_HEIGHT = HEIGHT / 40;


class Grid
{
public:
    const float tileWidth = 40.0;

    Grid(Engine *engine) {

        float halfWidth = (WIDTH - tileWidth) / 2.0f;
        float halfHeight = (HEIGHT - tileWidth) / 2.0f;

        std::cout << "Building grid (" << GRID_WIDTH << ", " << GRID_HEIGHT << ")" << std::endl;

        std::shared_ptr<Material> tileMat = std::make_shared<Material>(
            *engine->graphics->ctx, *engine->graphics->commandPool, *engine->graphics->swapChain);

        tileMat->setTexturePath("resources/textures/dirt.png");
        tileMat->setShader(
            *engine->graphics->swapChain, "resources/shaders/flat_vert.spv", "resources/shaders/flat_frag.spv");
        tileMat->setMaxMeshCount(10000);

        std::shared_ptr<MaterialComponent> tileMatComponent = std::make_shared<MaterialComponent>();
        tileMatComponent->material = tileMat;

        //std::random_device dev;
        //std::mt19937 rng(dev());
        //std::uniform_int_distribution<std::mt19937::result_type> dist10(1,15);

        for (int i = 0; i < GRID_HEIGHT; i++ ) {
            for (int j = 0; j < GRID_WIDTH; j++ ) {
                //std::stringstream s;
                //s << "tile_" << std::to_string(i) << "_" << std::to_string(j);
                grid[i][j] = std::make_shared<GameObject>();
                engine->graphics->addGameObject(grid[i][j]);

                float yPos = i * tileWidth - halfHeight;
                float xPos = j * tileWidth - halfWidth;

                std::shared_ptr<TransformComponent> tc = std::make_shared<TransformComponent>();

                tc->translate = glm::vec2(xPos, yPos);
                tc->scale = glm::vec2(tileWidth);

                grid[i][j]->addComponent(tc);

                std::shared_ptr<MeshComponent> mc = std::make_shared<MeshComponent>();
                std::shared_ptr<Mesh> square = std::make_shared<Square>(*engine->graphics->ctx);
                square->createBuffers(*engine->graphics->commandPool);
                mc->mesh = square;

                grid[i][j]->addComponent(mc);
                grid[i][j]->addComponent(tileMatComponent);
            }
        }
    }

    std::shared_ptr<GameObject> gameObjectAt(float worldY, float worldX) {
        float halfWidth = (WIDTH - tileWidth) / 2.0f;
        float halfHeight = (HEIGHT - tileWidth) / 2.0f;

        int scaledY = (worldY + halfHeight) / tileWidth;
        int scaledX = (worldX + halfWidth) / tileWidth;

        return grid[scaledY][scaledX];
    }

private:
    std::shared_ptr<GameObject> grid[GRID_HEIGHT][GRID_WIDTH];
};



class App : public AppBase
{
public:
    using AppBase::AppBase;

    App(unsigned int width, unsigned int height, int argc, char **argv) : AppBase(width, height, argc, argv)
    {
        grid = std::make_unique<Grid>(engine);

        ant1 = std::make_shared<AntGameObject>(engine);

        engine->graphics->addGameObject(ant1);


        //ant1->scale = glm::vec2(100.0f);
        //ant2->setMaterial(ant2Mat);

        //ant1leg = engine->graphics->addGameObject();
        //ant2leg = engine->graphics->addGameObject();
        //ant3leg = engine->graphics->addGameObject();
        //ant4leg = engine->graphics->addGameObject();
        //ant1legMat = engine->graphics->createSpriteMaterial("resources/textures/ant1_leg.png");
        //ant1leg->setMaterial(ant1legMat);
        //ant2leg->setMaterial(ant1legMat);
        //ant3leg->setMaterial(ant1legMat);
        //ant4leg->setMaterial(ant1legMat);

        //ant1leg->scale = glm::vec2(0.5f);
        //ant1leg->rotatePivot = glm::vec2(0.0f, 0.25f);

        //ant2leg->scale = glm::vec2(0.5f);
        //ant2leg->rotatePivot = glm::vec2(0.0f, 0.25f);

        //ant3leg->scale = glm::vec2(0.5f);
        //ant3leg->rotatePivot = glm::vec2(0.0f, -0.25f);

        //ant4leg->scale = glm::vec2(0.5f);
        //ant4leg->rotatePivot = glm::vec2(0.0f, -0.25f);

        //ant1leg->translate = glm::vec2(0.0f, -0.2f);
        //ant2leg->translate = glm::vec2(0.0f, -0.3f);
        //ant3leg->translate = glm::vec2(0.0f, +0.3f);
        //ant4leg->translate = glm::vec2(0.0f, +0.3f);

        //ant3leg->scale = glm::vec2(-1.0f);
        //ant4leg->scale = glm::vec2(-1.0f);

        //ant1leg->parentObject = ant1;
        //ant2leg->parentObject = ant1;
        //ant3leg->parentObject = ant1;
        //ant4leg->parentObject = ant1;

        //ant2->scale = glm::vec2(50.0f);
    }

    ~App()
    {
        //delete ant2Mat;
        //delete ant1legMat;
        //delete ant1leg;
        //delete ant2leg;
        //delete ant3leg;
        //delete ant4leg;
    }

    void update(float deltaTime) {
        ant1->update(deltaTime);
        //ant2->move(glm::vec2(0.0f), 200.0f * deltaTime);
        //ant1leg->rotate = glm::sin(glfwGetTime() * 5.0f) * 45.0f + 90.0f;
        //ant2leg->rotate = glm::cos(glfwGetTime() * 5.0f) * 45.0f + 90.0f;
        //ant3leg->rotate = glm::cos(glfwGetTime() * 5.0f) * 45.0f + 90.0f;
        //ant4leg->rotate = glm::sin(glfwGetTime() * 5.0f) * 45.0f + 90.0f;

        //GameObject* square = grid->gameObjectAt(ant1->translate.y, ant1->translate.x);

        //glm::vec3 randColor = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
        //square->color = randColor;
    }

    void processInput(float deltaTime) {
        if (keys[GLFW_KEY_ESCAPE]) {
            exitApp();
            return;
        }

        float velocity = 300.0f * deltaTime;
        float moveX = 0, moveY = 0, rotate = 0;

        std::shared_ptr<TransformComponent> tc = ant1->getComponent<TransformComponent>();

        if (keys[GLFW_KEY_W])
        {
            moveX += -velocity * cos(glm::radians(90.0 - tc->rotate));
            moveY += velocity * sin(glm::radians(90.0 - tc->rotate));
        }
        if (keys[GLFW_KEY_S])
        {
            moveX += velocity * cos(glm::radians(90.0 - tc->rotate));
            moveY += -velocity * sin(glm::radians(90.0 - tc->rotate));
        }
        if (keys[GLFW_KEY_A])
        {
            rotate += velocity * 0.8;
        }
        if (keys[GLFW_KEY_D])
        {
            rotate -= velocity * 0.8;
        }
        // Move the square.
        if (moveX != 0 | moveY != 0 | rotate != 0) {
            tc->move(glm::vec2(moveX, moveY), rotate);
        }
    }

private:
    std::shared_ptr<AntGameObject> ant1;
    //GameObject* ant2;
    //GameObject* ant1leg;
    //GameObject* ant2leg;
    //GameObject* ant3leg;
    //GameObject* ant4leg;
    //Material *ant2Mat;
    //Material *ant1legMat;
    std::unique_ptr<Grid> grid;
};


int main(int argc, char** argv) {
    try {
        App app = App(WIDTH, HEIGHT, argc, argv);
        app.mainLoop();
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
};
