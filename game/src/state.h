#pragma once

#include "map/map.h"
#include "splash.h"
#include "raylib.h"

typedef struct {
    Map map;
    Camera2D camera;
    SplashMessages splash_messages;
} GameState;

void open_map(GameState* state);