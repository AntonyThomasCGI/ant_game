#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float ANT_VELOCITY(270.0f);
const float ANT_SIZE(60.0f);
const float HOTDOG_SIZE(40.f);
const float NEST_SIZE(160.0f);

enum GameState {
    GAME_ACTIVE,
    GAME_MENU
};


class Game
{
public:
    GameState state;
    bool keys[1024];
    bool keysProcessed[1024];
    unsigned int Width;
    unsigned int Height;

    bool wireFrameMode = false;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void init();
    void processInput(float dt);
    void update(float dt);
    void render();
    void resize(int width, int height);
    void doCollisions();
};

#endif
