#pragma once

#include "map.h"
#include "raylib.h"

typedef struct {
    Map map;
    Camera2D camera;
} GameState;

void open_map(GameState* state);