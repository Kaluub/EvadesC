#pragma once

#include "map.h"
#include "raylib.h"

typedef struct {
    Map map;
    Camera2D camera;
} GameState;