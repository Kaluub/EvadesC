#include "circle.h"
#include "raymath.h"
#include <stdlib.h>
#include <assert.h>

static Texture2D circle_texture = {0};
static Rectangle circle_rect = {0, 0, 4096, 4096};

void init_circle_texture() {
    Image circle_image = GenImageColor(4096, 4096, GetColor(0));
    ImageDrawCircle(&circle_image, circle_image.width/2, circle_image.height/2, circle_image.width/2, WHITE);
    circle_texture = LoadTextureFromImage(circle_image);
    GenTextureMipmaps(&circle_texture);
    UnloadImage(circle_image);
}

void draw_circle(Vector2 position, float radius, Color color) {
    if (IsKeyDown(KEY_V)) {
        color = ColorAlpha(color, 0.3f);
    }
    DrawTexturePro(
        circle_texture,
        circle_rect,
        (Rectangle) {position.x - radius, position.y - radius, radius*2, radius*2},
        Vector2Zero(),
        0,
        color
    );
}