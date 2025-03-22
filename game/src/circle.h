#pragma once

#include "raylib.h"

typedef struct Circle {
    Vector2 position;
    float radius;
    Color color;
} Circle;

typedef struct CircleCollection {
    Circle* circles;
    int stored_circles;
    int capacity;
} CircleCollection;

void init_circle_texture();
void init_circles(CircleCollection *collection, int capacity);
void add_circle(CircleCollection* collection, Circle circle);
void draw_circles(const CircleCollection *collection);
void cleanup_circles(CircleCollection *collection);