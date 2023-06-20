
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "game.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "game_object.hpp"
#include "ant.hpp"
#include "map.hpp"
#include "chunk_renderer.hpp"

#include <iostream>
#include <time.h>
#include <random>


SpriteRenderer *Renderer;
AntObject *Ant;
GameObject *Hotdog;
GameObject *HomeNest;
ChunkRenderer *MapRenderer;


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
    delete MapRenderer;
}


void Game::init()
{
    srand(static_cast<unsigned int>(time (NULL)));  // Randomize seed

    glLineWidth(3.0f);

    ResourceManager::LoadShader("C:\\dev\\ant_game\\resources\\sprite.vert", "C:\\dev\\ant_game\\resources\\sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    int checkWidth = 16;
    int checkHeight = 16;
    //ResourceManager::LoadShader("C:\\dev\\ant_game\\resources\\checker.vert", "C:\\dev\\ant_game\\resources\\checker.frag", nullptr, "checker");
    //ResourceManager::GetShader("checker").Use().SetInteger("checker", 0);
    //ResourceManager::GetShader("checker").SetMatrix4("projection", projection);
    int** mapData = Map::Generate(checkWidth, checkHeight, 41, 2);

    Texture2D mapTexture;
    mapTexture.Filter_Min = GL_NEAREST;
    mapTexture.Filter_Max = GL_NEAREST;
    mapTexture.Internal_Format = GL_R32I;
    mapTexture.Image_Format = GL_RED_INTEGER;
    mapTexture.Generate(checkWidth, checkHeight, mapData);
    ResourceManager::Textures["map"] = mapTexture;
    //MapRenderer = new SpriteRenderer(ResourceManager::GetShader("checker"));
    free(mapData);

    Texture2D dirtTex = ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\dirt_tiles.png", true, "dirt_tiles");
    Shader tileShader = ResourceManager::LoadShader("C:\\dev\\ant_game\\resources\\tile.vert", "C:\\dev\\ant_game\\resources\\tile.frag", nullptr, "tile");

    tileShader.Use().SetInteger("tileTex", 0);
    tileShader.SetInteger("mapTex", 1);
    tileShader.SetVector2f("textureDimensions", glm::vec2(dirtTex.Width, dirtTex.Height));
    tileShader.SetMatrix4("projection", projection);
    MapRenderer = new ChunkRenderer(tileShader);


    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\ant.png", true, "ant");
    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\background.jpg", false, "background");
    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\hotdog.png", true, "food");
    ResourceManager::LoadTexture("C:\\dev\\ant_game\\resources\\sandcastle.png", true, "sandcastle");

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    glm::vec2 antPos = glm::vec2(this->Width / 2.0f - 100.0, this->Height / 2.0f - 300.0);
    Ant = new AntObject(antPos, glm::vec2(ANT_SIZE), ResourceManager::GetTexture("ant"));

    glm::vec2 hotdogPos = glm::vec2(glm::linearRand(50.0f, this->Width - 100.0f), glm::linearRand(100.0f, this->Height - 50.0f));
    Hotdog = new GameObject(hotdogPos, glm::vec2(HOTDOG_SIZE), ResourceManager::GetTexture("food"));

    glm::vec2 nestPos = glm::vec2(glm::linearRand(50.0f, this->Width - 100.0f), glm::linearRand(100.0f, this->Height - 50.0f));
    HomeNest = new GameObject(nestPos, glm::vec2(NEST_SIZE), ResourceManager::GetTexture("sandcastle"));
}


void Game::processInput(float dt)
{
    float velocity = ANT_VELOCITY * dt;

    if (this->keys[GLFW_KEY_W] && !this->keysProcessed[GLFW_KEY_W])
    {
        Ant->Position.x += velocity * cos(glm::radians(90.0 - Ant->Rotation));
        Ant->Position.y -= velocity * sin(glm::radians(90.0 - Ant->Rotation));
    }
    if (this->keys[GLFW_KEY_S] && !this->keysProcessed[GLFW_KEY_S])
    {
        Ant->Position.x -= velocity * cos(glm::radians(90.0 - Ant->Rotation));
        Ant->Position.y += velocity * sin(glm::radians(90.0 - Ant->Rotation));
    }
    if (this->keys[GLFW_KEY_A] && !this->keysProcessed[GLFW_KEY_A])
    {
        Ant->Rotation -= velocity;
    }
    if (this->keys[GLFW_KEY_D] && !this->keysProcessed[GLFW_KEY_D])
    {
        Ant->Rotation += velocity;
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
    MapRenderer->DrawChunk(ResourceManager::GetTexture("dirt_tiles"), ResourceManager::GetTexture("map"), glm::vec2(100.0f), glm::vec2(700.0f, 700.0f));
    //MapRenderer->DrawSprite(ResourceManager::GetTexture("map"), glm::vec2(0.0f), glm::vec2(this->Width, this->Height));
    HomeNest->Draw(*Renderer);
    //Renderer->DrawSprite(ResourceManager::GetTexture("ant"), glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(0.8f, 0.7f, 0.1));
    //Renderer->DrawSprite(ResourceManager::GetTexture("ant"), glm::vec2(this->Width / 2.0f - 100.0, this->Height / 2.0f - 300.0), glm::vec2(400.0f), 45.0f);
    Ant->Draw(*Renderer);
    Hotdog->Draw(*Renderer);
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
}

bool checkCollision(GameObject *obj1, GameObject *obj2)
{
    bool collisionX = obj1->Position.x + obj1->Size.x >= obj2->Position.x &&
        obj2->Position.x + obj2->Size.x >= obj1->Position.x;

    bool collisionY = obj1->Position.y + obj1->Size.y >= obj2->Position.y &&
        obj2->Position.y + obj2->Size.y >= obj1->Position.y;

    return collisionX && collisionY;
}
