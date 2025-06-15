
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



const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const unsigned int GRID_WIDTH = WIDTH / 20;
const unsigned int GRID_HEIGHT = HEIGHT / 20;


const std::string dirtSprite = "resources/textures/dirt.png";


class Grid
{
public:
    const float tileWidth = 20.0;

    Grid(Engine *engine) {

        float halfWidth = (WIDTH - tileWidth) / 2.0f;
        float halfHeight = (HEIGHT - tileWidth) / 2.0f;

        std::cout << "Building grid (" << GRID_WIDTH << ", " << GRID_HEIGHT << ")" << std::endl;

        tileMat = engine->graphics->createSpriteMaterial(dirtSprite);
        tileMat->setMaxMeshCount(10000);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist10(1,15);

        for (int i = 0; i < GRID_HEIGHT; i++ ) {
            for (int j = 0; j < GRID_WIDTH; j++ ) {
                //std::stringstream s;
                //s << "tile_" << std::to_string(i) << "_" << std::to_string(j);
                grid[i][j] = engine->graphics->addGameObject();
                grid[i][j]->setMaterial(tileMat);
                //grid[i][j]->setSpritePath(dirtSprite);

                float yPos = i * tileWidth - halfHeight;
                float xPos = j * tileWidth - halfWidth;

                grid[i][j]->translate = glm::vec2(xPos, yPos);
<<<<<<< HEAD
                grid[i][j]->scale = glm::vec2(tileWidth);

                // Sometimes, set a grid tile a random color.
                //if (dist10(rng) > 14) {
                //    glm::vec3 randColor = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
                //    grid[i][j]->color = randColor;
                //}
=======
                grid[i][j]->baseSize = glm::vec2(tileWidth);
>>>>>>> d6c7017 (fix validations, add baseSize to game object)
            }
        }
    }

    GameObject* gameObjectAt(float worldY, float worldX) {
        float halfWidth = (WIDTH - tileWidth) / 2.0f;
        float halfHeight = (HEIGHT - tileWidth) / 2.0f;

        int scaledY = (worldY + halfHeight) / tileWidth;
        int scaledX = (worldX + halfWidth) / tileWidth;

        return grid[scaledY][scaledX];
    }

    ~Grid() {
        delete tileMat;
    }

private:
    GameObject* grid[GRID_HEIGHT][GRID_WIDTH];
    Material *tileMat;
};



class App : public AppBase
{
public:
    using AppBase::AppBase;

    App(unsigned int width, unsigned int height, int argc, char **argv) : AppBase(width, height, argc, argv)
    {

        grid = std::make_unique<Grid>(engine);

        ant1 = engine->graphics->addGameObject();
        //ant2 = engine->graphics->addGameObject();


        ant1Mat = engine->graphics->createSpriteMaterial("resources/textures/ant1_no_leg.png");
        //ant2Mat = engine->graphics->createSpriteMaterial("resources/textures/ant2.png");

        ant1->setMaterial(ant1Mat);
        ant1->baseSize = glm::vec2(100.0f);
        //ant2->setMaterial(ant2Mat);

        ant1leg = engine->graphics->addGameObject();
        ant2leg = engine->graphics->addGameObject();
        ant3leg = engine->graphics->addGameObject();
        ant4leg = engine->graphics->addGameObject();
        ant1legMat = engine->graphics->createSpriteMaterial("resources/textures/ant1_leg.png");
        ant1leg->setMaterial(ant1legMat);
        ant2leg->setMaterial(ant1legMat);
        ant3leg->setMaterial(ant1legMat);
        ant4leg->setMaterial(ant1legMat);

        ant1leg->baseSize = glm::vec2(0.5f);
        ant1leg->rotatePivot = glm::vec2(0.0f, 0.25f);

        ant2leg->baseSize = glm::vec2(0.5f);
        ant2leg->rotatePivot = glm::vec2(0.0f, 0.25f);

        ant3leg->baseSize = glm::vec2(0.5f);
        ant3leg->rotatePivot = glm::vec2(0.0f, -0.25f);

        ant4leg->baseSize = glm::vec2(0.5f);
        ant4leg->rotatePivot = glm::vec2(0.0f, -0.25f);

        ant1leg->translate = glm::vec2(0.0f, -0.2f);
        ant2leg->translate = glm::vec2(0.0f, -0.3f);
        ant3leg->translate = glm::vec2(0.0f, +0.3f);
        ant4leg->translate = glm::vec2(0.0f, +0.3f);

        ant3leg->scale = glm::vec2(-1.0f);
        ant4leg->scale = glm::vec2(-1.0f);

        ant1leg->parentObject = ant1;
        ant2leg->parentObject = ant1;
        ant3leg->parentObject = ant1;
        ant4leg->parentObject = ant1;

        //ant2->baseSize = glm::vec2(50.0f);
    }

    ~App()
    {
        delete ant1Mat;
        //delete ant2Mat;
        delete ant1legMat;
        delete ant1leg;
        delete ant2leg;
        delete ant3leg;
        delete ant4leg;
    }

    void update(float deltaTime) {
        //ant2->move(glm::vec2(0.0f), 200.0f * deltaTime);
        ant1leg->rotate = glm::sin(glfwGetTime() * 5.0f) * 45.0f + 90.0f;
        ant2leg->rotate = glm::cos(glfwGetTime() * 5.0f) * 45.0f + 90.0f;
        ant3leg->rotate = glm::cos(glfwGetTime() * 5.0f) * 45.0f + 90.0f;
        ant4leg->rotate = glm::sin(glfwGetTime() * 5.0f) * 45.0f + 90.0f;

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

        if (keys[GLFW_KEY_W] && !keysProcessed[GLFW_KEY_W])
        {
            moveX += -velocity * cos(glm::radians(90.0 - ant1->rotate));
            moveY += velocity * sin(glm::radians(90.0 - ant1->rotate));
        }
        if (keys[GLFW_KEY_S] && !keysProcessed[GLFW_KEY_S])
        {
            moveX += velocity * cos(glm::radians(90.0 - ant1->rotate));
            moveY += -velocity * sin(glm::radians(90.0 - ant1->rotate));
        }
        if (keys[GLFW_KEY_A] && !keysProcessed[GLFW_KEY_A])
        {
            rotate += velocity * 0.8;
        }
        if (keys[GLFW_KEY_D] && !keysProcessed[GLFW_KEY_D])
        {
            rotate -= velocity * 0.8;
        }
        // Move the square.
        if (moveX != 0 | moveY != 0 | rotate != 0) {
            ant1->move(glm::vec2(moveX, moveY), rotate);
        }

        if (keys[GLFW_KEY_E] && !keysProcessed[GLFW_KEY_E]) {
            glm::vec3 newColor = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
            ant1->color = newColor;
            keysProcessed[GLFW_KEY_E] = true;
        }
    }

private:
    GameObject* ant1;
    //GameObject* ant2;
    GameObject* ant1leg;
    GameObject* ant2leg;
    GameObject* ant3leg;
    GameObject* ant4leg;
    Material *ant1Mat;
    //Material *ant2Mat;
    Material *ant1legMat;
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
