#pragma once

#include "map.h"
#include "data/unordered_list.h"
#include "raylib.h"

typedef struct {
    Map map;
    UnorderedList loaded_areas;
    Camera2D camera;
} GameState;