
#pragma once

#include "engine.hpp"
#include "game_object.hpp"


class Locator : public GameObject
{

public:
    Locator(Engine *engine, float x, float y);
};

