
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "game_object.hpp"
#include "ant.hpp"
#include "map.hpp"

#include <iostream>
#include <time.h>
#include <random>


SpriteRenderer *Renderer;
AntObject *Ant;
GameObject *Hotdog;
GameObject *HomeNest;
Map *map;


Game::Game(unsigned int width, unsigned int height)
    : state(GAME_ACTIVE), keys(), Width(width), Height(height)
{

}


Game::~Game()
{
    delete Ant;
    delete Renderer;
    delete Hotdog;
    delete HomeNest;
    delete map;
}


void Game::init()
{
    srand(static_cast<unsigned int>(time (NULL)));  // Randomize seed

    // Wireframe line width
    glLineWidth(3.0f);

    Shader spriteShader = ResourceManager::LoadShader("C:\\dev\\ant_game\\resources\\sprite.vert", "C:\\dev\\ant_game\\resources\\sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    spriteShader.Use().SetInteger("sprite", 0);
    spriteShader.SetMatrix4("projection", projection);

    Shader chunkShader = ResourceManager::LoadShader("C:\\dev\\ant_game\\resources\\chunk.vert", "C:\\dev\\ant_game\\resources\\chunk.frag", nullptr, "chunk");
    chunkShader.Use().SetInteger("tileTex", 0);
    chunkShader.SetMatrix4("projection", projection);

    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\ant.png", true, "ant");
    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\hotdog.png", true, "food");
    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\sandcastle.png", true, "sandcastle");
    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\dirt_tiles.png", true, "dirt_tiles");

    map = new Map(160, 160, chunkShader, 42, 5);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    // Start ant at 0, 0. If a wall exists, increment x axis until no wall.
    glm::vec2 antPos = glm::vec2(0.0f);
    int i = 0;
    while ( 1 ) {
        int tile = map->tileAt(antPos);
        if (tile != 0 && tile != 8) {
            break;
        }
        antPos = glm::vec2(antPos.x + TILESIZE, antPos.y);
    }
    Ant = new AntObject(antPos, glm::vec2(ANT_SIZE), ResourceManager::GetTexture("ant"));

    glm::vec2 hotdogPos = glm::vec2(glm::linearRand(0.0f, float(this->Width)), glm::linearRand(0.0f, float(this->Height)));
    Hotdog = new GameObject(hotdogPos, glm::vec2(HOTDOG_SIZE), ResourceManager::GetTexture("food"));

    glm::vec2 nestPos = glm::vec2(glm::linearRand(0.0f, float(this->Width)), glm::linearRand(0.0f, float(this->Height)));
    HomeNest = new GameObject(nestPos, glm::vec2(NEST_SIZE), ResourceManager::GetTexture("sandcastle"));
}


void Game::processInput(float dt)
{
    float velocity = ANT_VELOCITY * dt;

    float moveX = 0, moveY = 0, rotate = 0;

    if (this->keys[GLFW_KEY_W] && !this->keysProcessed[GLFW_KEY_W])
    {
        moveX += velocity * cos(glm::radians(90.0 - Ant->Rotation));
        moveY += -velocity * sin(glm::radians(90.0 - Ant->Rotation));
    }
    if (this->keys[GLFW_KEY_S] && !this->keysProcessed[GLFW_KEY_S])
    {
        moveX += -velocity * cos(glm::radians(90.0 - Ant->Rotation));
        moveY += velocity * sin(glm::radians(90.0 - Ant->Rotation));
    }
    if (this->keys[GLFW_KEY_A] && !this->keysProcessed[GLFW_KEY_A])
    {
        rotate -= velocity * 0.8;
    }
    if (this->keys[GLFW_KEY_D] && !this->keysProcessed[GLFW_KEY_D])
    {
        rotate += velocity * 0.8;
    }
    // Move the Ant.
    if (moveX != 0 | moveY != 0 | rotate != 0) {
        Ant->move(glm::vec2(moveX, moveY), rotate);
    }

    if (this->keys[GLFW_KEY_F3] && !this->keysProcessed[GLFW_KEY_F3])
    {
        // Toggle wire frame
        if (this->wireFrameMode) {
            this->wireFrameMode = false;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            this->wireFrameMode = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        this->keysProcessed[GLFW_KEY_F3] = true;
    }
}

void Game::resize(int width, int height)
{
    this->Width = width;
    this->Height = height;
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
}


void Game::update(float dt)
{
    doCollisions();

    // Update held item positions.
    for (auto item : Ant->heldItems) {
        item->Position = Ant->Position;
    }
}


void Game::render()
{
    // Camera follows ant, so inverse ant current position + half screen width/height to center.
    glm::vec2 cameraTransform = Ant->Position * -1.0f + glm::vec2(this->Width / 2.0f, this->Height / 2.0f);
    map->drawChunks(cameraTransform);
    //MapRenderer->DrawChunk(ResourceManager::GetTexture("dirt_tiles"), glm::vec2(100.0f), glm::vec2(700.0f, 700.0f));
    //MapRenderer->DrawSprite(ResourceManager::GetTexture("map"), glm::vec2(0.0f), glm::vec2(this->Width, this->Height));
    HomeNest->Draw(*Renderer, cameraTransform);
    //Renderer->DrawSprite(ResourceManager::GetTexture("ant"), glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.8f, 0.7f, 0.1));
    //Renderer->DrawSprite(ResourceManager::GetTexture("ant"), glm::vec2(this->Width / 2.0f - 100.0, this->Height / 2.0f - 300.0), glm::vec2(400.0f), 45.0f);
    Ant->Draw(*Renderer, cameraTransform);
    Hotdog->Draw(*Renderer, cameraTransform);
    //Renderer->DrawSprite(ResourceManager::GetTexture("food"), glm::vec2(300.f, 400.0f), glm::vec2(50.0f, 50.0f));
}

bool checkCollision(GameObject *obj1, GameObject *obj2);

void Game::doCollisions()
{
    if (!Ant->heldItems.size()) {
        if (checkCollision(Ant, Hotdog)) {
            Ant->addHeldItem(Hotdog);
        }
    }

    /*
    We want to generate a hit box that looks like this:
    0............1
    ..............
    ..............
    ..............
    ..............
    ..............
    ..............
    .......A......
    ..............
    ..............
    ..............
    ..............
    ..............
    2............3

    So solve for a/b which at 90 degrees ant rotation will be 6 and 18 respectively.

              .
             /|
            /A|
           /  |
          /   |
         /    |
      c /     | b
       /      |
      /       |
     /       _|
    /B______|_|
        a
    */

    float angleA = atan(6.0 / 18.0);
    float hypot = sqrt(pow(18.0, 2) + pow(6.0, 2));
    float hitBox0XOffset = hypot * cos(glm::radians(90.0 - Ant->Rotation) + angleA);
    float hitBox0YOffset = hypot * sin(glm::radians(90.0 - Ant->Rotation) + angleA);
    float hitBox1XOffset = hypot * cos(glm::radians(90.0 - Ant->Rotation) - angleA);
    float hitBox1YOffset = hypot * sin(glm::radians(90.0 - Ant->Rotation) - angleA);
    float hitBox0X = Ant->Position.x + hitBox0XOffset;
    float hitBox0Y = Ant->Position.y + hitBox0YOffset;
    float hitBox1X = Ant->Position.x + hitBox1XOffset;
    float hitBox1Y = Ant->Position.y + hitBox1YOffset;
    float hitBox2X = Ant->Position.x + hitBox1XOffset * -1.0;
    float hitBox2Y = Ant->Position.y + hitBox1YOffset * -1.0;
    float hitBox3X = Ant->Position.x + hitBox0XOffset * -1.0;
    float hitBox3Y = Ant->Position.y + hitBox0YOffset * -1.0;

    int current0 = map->tileAt(glm::vec2(hitBox0X, hitBox0Y));
    int current1 = map->tileAt(glm::vec2(hitBox1X, hitBox1Y));
    int current2 = map->tileAt(glm::vec2(hitBox2X, hitBox2Y));
    int current3 = map->tileAt(glm::vec2(hitBox3X, hitBox3Y));

    // Tile ID of 0 or 8 is wall.
    if (current0 == 0 || current1 == 0 || current2 == 0 || current3 == 0 ||
        current0 == 8 || current1 == 8 || current2 == 8 || current3 == 8) {
        Ant->Position = Ant->previousPosition;
        Ant->Rotation = Ant->previousRotation;
    }
}

bool checkCollision(GameObject *obj1, GameObject *obj2)
{
    bool collisionX = obj1->Position.x + obj1->Size.x >= obj2->Position.x &&
        obj2->Position.x + obj2->Size.x >= obj1->Position.x;

    bool collisionY = obj1->Position.y + obj1->Size.y >= obj2->Position.y &&
        obj2->Position.y + obj2->Size.y >= obj1->Position.y;

    return collisionX && collisionY;
}
