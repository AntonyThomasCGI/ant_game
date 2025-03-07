
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

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

    Grid(Engine &engine) {

        float halfWidth = (WIDTH - tileWidth) / 2.0f;
        float halfHeight = (HEIGHT - tileWidth) / 2.0f;

        std::cout << "Building grid (" << GRID_WIDTH << ", " << GRID_HEIGHT << ")" << std::endl;

        for (int i = 0; i < GRID_HEIGHT; i++ ) {
            for (int j = 0; j < GRID_WIDTH; j++ ) {
                std::stringstream s;
                s << "tile_" << std::to_string(i) << "_" << std::to_string(j);
                grid[i][j] = engine.graphics->addGameObject(s.str());

                float yPos = i * tileWidth - halfHeight;
                float xPos = j * tileWidth - halfWidth;

                grid[i][j]->translate = glm::vec2(xPos, yPos);
                grid[i][j]->scale = glm::vec2(tileWidth);


                glm::vec3 randColor = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
                grid[i][j]->color = randColor;
            }
        }
    }

    void update() {
        for (int i = 0; i < GRID_HEIGHT; i++ ) {
            for (int j = 0; j < GRID_WIDTH; j++ ) {
                glm::vec3 currentColor = grid[i][j]->color;

                glm::vec3 offset = glm::vec3(glm::linearRand(-0.05f, 0.05f), glm::linearRand(-0.05f, 0.05f), glm::linearRand(-0.05f, 0.05f));

                glm::vec3 result = glm::clamp(currentColor + offset, 0.0f, 1.0f);

                grid[i][j]->color = result;
            }
        }
    }

private:
    GameObject* grid[GRID_HEIGHT][GRID_WIDTH];
};



class App : public AppBase
{
public:
    using AppBase::AppBase;

    App(unsigned int width, unsigned int height) : AppBase(width, height)
    {
        //grid = new Grid(engine);
        squareGuy = engine->graphics->addGameObject("z");
        squareGuy1 = engine->graphics->addGameObject("z1");
    }

    ~App()
    {
        //delete grid;
    }

    void update(float deltaTime) {
        squareGuy1->move(glm::vec2(0.0f), 200.0f * deltaTime);

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
            moveX += -velocity * cos(glm::radians(90.0 - squareGuy->rotate));
            moveY += velocity * sin(glm::radians(90.0 - squareGuy->rotate));
        }
        if (keys[GLFW_KEY_S] && !keysProcessed[GLFW_KEY_S])
        {
            moveX += velocity * cos(glm::radians(90.0 - squareGuy->rotate));
            moveY += -velocity * sin(glm::radians(90.0 - squareGuy->rotate));
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
            squareGuy->move(glm::vec2(moveX, moveY), rotate);
        }

        if (keys[GLFW_KEY_E] && !keysProcessed[GLFW_KEY_E]) {
            glm::vec3 newColor = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
            squareGuy->color = newColor;
            keysProcessed[GLFW_KEY_E] = true;
        }

        if (keys[GLFW_KEY_Q] && !keysProcessed[GLFW_KEY_Q]) {
            grid->update();
        }
    }

private:
    GameObject* squareGuy;
    GameObject* squareGuy1;
    Grid *grid;
};


int main() {
    std::cout << "start" << std::endl;

    try {
        App app = App(WIDTH, HEIGHT);
        app.mainLoop();
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    std::cout << "end" << std::endl;

    return EXIT_SUCCESS;
};
