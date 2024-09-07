
#include <chrono>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/app.hpp"
#include "engine/engine.hpp"


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


class App : public AppBase
{
public:
    using AppBase::AppBase;

    App(Window &window, Engine &engine) : AppBase(window, engine)
    {
        squareGuy = engine.graphics->addGameObject("squareGuy");
        squareGuy1 = engine.graphics->addGameObject("squareGuy1");
    }

    void update(float deltaTime) {

        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        float rotation = time * glm::radians(90.0f) * 100.0f;

        squareGuy1->rotate = rotation;

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
    }

private:
    GameObject* squareGuy;
    GameObject* squareGuy1;
};


int main() {
    std::cout << "start" << std::endl;

    try {
        Window window = Window(WIDTH, HEIGHT);
        Engine engine = Engine(window);
        App app = App(window, engine);
        app.mainLoop();
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    std::cout << "end" << std::endl;

    return EXIT_SUCCESS;
};
