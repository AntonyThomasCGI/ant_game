#pragma once

#include "devices/window.hpp"
#include "engine.hpp"


class AppBase
{
public:
    AppBase(unsigned int width, unsigned int height, int argc, char **argv);
    ~AppBase();

    virtual void processInput(float deltaTime) = 0;
    virtual void update(float deltaTime) = 0;

    void mainLoop();

    void exitApp();

    bool keys[1024] = { false };
    bool keysProcessed[1024] = { false };

    Window *window;
    Engine *engine;

private:

    void _processInput(int key, int scancode, int action, int mode);

    bool running = true;

    rocket::scoped_connection_container connections;
};
