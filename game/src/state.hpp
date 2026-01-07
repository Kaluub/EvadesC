#pragma once

#include "map/map.hpp"
#include "splash.hpp"
#include "debug.hpp"
#include "raylib.h"

class GameState {
public:
    Map map;
    Camera2D camera;
    SplashMessages splash_messages;
    DebugState debug_state;

    void read_map();
};