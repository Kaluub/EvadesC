#include "circle.h"
#include "raymath.h"
#include <stdlib.h>

static Texture2D circle_texture = {0};
static Rectangle circle_rect = {0, 0, 4096, 4096};

// void populate_with_debug_circles(CircleCollection *collection) {
//     int n = 0;
//     for (int i = 0; i < 500; i++) {
//         for (int j = 0; j < 250; j++) {
//             collection->circles[n] = (Circle) {
//                 (Vector2) {i * 500, j * 500},
//                 GetRandomValue(50, 100),
//                 GetColor(GetRandomValue(0, 0xFFFFFF) << 8 | 0xFF)
//             };
//             n++;
//         }
//     }
//     collection->stored_circles += n;
// }

void init_circles(CircleCollection *collection, int capacity) {
    if (circle_texture.id == 0) {
        Image circle_image = GenImageColor(4096, 4096, GetColor(0));
        ImageDrawCircle(&circle_image, circle_image.width/2, circle_image.height/2, circle_image.width/2, WHITE);
        circle_texture = LoadTextureFromImage(circle_image);
        GenTextureMipmaps(&circle_texture);
        UnloadImage(circle_image);
    }
    collection->circles = (Circle*) malloc(sizeof(Circle) * capacity);
    collection->stored_circles = 0;
    collection->capacity = capacity;
    // populate_with_debug_circles(collection);
}

void draw_circles(CircleCollection *collection) {
    for (int i = 0; i < collection->stored_circles; i++) {
        Circle circle = collection->circles[i];
        DrawTexturePro(
            circle_texture,
            circle_rect,
            (Rectangle) {circle.position.x - circle.radius, circle.position.y - circle.radius, circle.radius*2, circle.radius*2},
            Vector2Zero(),
            0,
            circle.color
        );
    }
}

void cleanup_circles(CircleCollection *collection) {
    free(collection->circles);
    collection->stored_circles = 0;
    collection->capacity = 0;
}