
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

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist10(1,15);

        for (int i = 0; i < GRID_HEIGHT; i++ ) {
            for (int j = 0; j < GRID_WIDTH; j++ ) {
                std::stringstream s;
                s << "tile_" << std::to_string(i) << "_" << std::to_string(j);
                grid[i][j] = engine->graphics->addGameObject(s.str());
                grid[i][j]->setSpritePath("resources/textures/dirt.png");

                float yPos = i * tileWidth - halfHeight;
                float xPos = j * tileWidth - halfWidth;

                grid[i][j]->translate = glm::vec2(xPos, yPos);
                grid[i][j]->scale = glm::vec2(tileWidth);

                // Sometimes, set a grid tile a random color.
                //if (dist10(rng) > 14) {
                //    glm::vec3 randColor = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
                //    grid[i][j]->color = randColor;
                //}
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

private:
    GameObject* grid[GRID_HEIGHT][GRID_WIDTH];
};



class App : public AppBase
{
public:
    using AppBase::AppBase;

    App(unsigned int width, unsigned int height, int argc, char **argv) : AppBase(width, height, argc, argv)
    {
        grid = std::make_unique<Grid>(engine);
        ant1 = engine->graphics->addGameObject("z");
        ant2 = engine->graphics->addGameObject("z1");

        ant1->setSpritePath("resources/textures/ant1.png");
        ant2->setSpritePath("resources/textures/ant2.png");
    }

    ~App()
    {
    }

    void update(float deltaTime) {
        ant2->move(glm::vec2(0.0f), 200.0f * deltaTime);
        //std::cout << glm::to_string(ant1->translate) << std::endl;
        GameObject* square = grid->gameObjectAt(ant1->translate.y, ant1->translate.x);

        glm::vec3 randColor = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
        square->color = randColor;
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
    GameObject* ant2;
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
