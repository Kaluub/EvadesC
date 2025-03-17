#include "gpu_circle.h"
#include "rlgl.h"
#include <stdlib.h>

#define MAX_CIRCLES 128000

GPUCircleCollection collection;

void populate_with_debug_circles() {
    int n = 0;
    for (int i = 0; i < 250; i++) {
        for (int j = 0; j < 250; j++) {
            collection.circles[n] = (GPUCircle) {
                (Vector2) {i * 500, j * 500},
                GetRandomValue(50, 100),
                GetColor(GetRandomValue(0, 0xFFFFFF) << 8 | 0xFF)
            };
            n++;
        }
    }
    collection.stored_circles += n;
}

void init_circles() {
    collection.circles = (GPUCircle*) malloc(sizeof(GPUCircle) * MAX_CIRCLES);
    collection.stored_circles = 0;
    populate_with_debug_circles();
}

void draw_circles() {
    for (int i = 0; i < collection.stored_circles; i++) {
        DrawCircleV(collection.circles[i].position, collection.circles[i].radius, collection.circles[i].color);
    }
}

void cleanup_circles() {
    free(collection.circles);
}

GPUCircleCollection* get_circle_collection() {
    return &collection;
}