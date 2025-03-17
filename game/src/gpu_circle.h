#pragma once

#include "raylib.h"

typedef struct GPUCircle {
    Vector2 position;
    float radius;
    Color color;
} GPUCircle;

typedef struct GPUCircleCollection {
    GPUCircle* circles;
    int stored_circles;
} GPUCircleCollection;

void init_circles();
void draw_circles();
void cleanup_circles();
GPUCircleCollection* get_circle_collection();