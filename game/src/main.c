#include "state.h"
#include <stdlib.h>
#include <time.h>

#define BACKGROUND_COLOR (Color) {0x33, 0x33, 0x33, 0xFF}
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

const Color ZONE_COLORS[] = {
    (Color) {0, 0, 0, 255},
    (Color) {195, 195, 195, 255},
    (Color) {255, 255, 255, 255},
    (Color) {255, 244, 108, 255},
    (Color) {106, 208, 222, 255},
    (Color) {255, 244, 108, 255},
    (Color) {255, 249, 186, 255},
    (Color) {255, 255, 255, 255},
};

int is_zone_on_screen(Camera2D camera, const Zone zone) {
#ifdef DEBUG
    if (IsKeyDown(KEY_B)) {
        camera.offset = (Vector2) {-WINDOW_WIDTH/2, -WINDOW_HEIGHT/2};
        camera.zoom *= 2;
    }
#endif
    Vector2 top_left = GetWorldToScreen2D((Vector2) {zone.x, zone.y}, camera);
    Vector2 bottom_right = GetWorldToScreen2D((Vector2) {zone.x + zone.width, zone.y + zone.height}, camera);

    return ((bottom_right.x >= 0 && top_left.x <= WINDOW_WIDTH) && (bottom_right.y >= 0 && top_left.y <= WINDOW_HEIGHT));
}

int main() {
    GameState state;
    FILE* file = fopen("maps/world.bin", "rb");

    load_map(&state.map, file);
    fclose(file);
    printf("Loaded spawn region name: %s\nLoaded regions: %d\n", state.map.spawn_region, state.map.region_count);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Evades");

    state.camera.offset = (Vector2) {0, 0};
    state.camera.rotation = 0;
    state.camera.target = (Vector2) {0, 0};
    state.camera.zoom = 1;

    float speed = 500 / state.camera.zoom;

    Image tile_image = GenImageColor(32, 32, WHITE);
    ImageDrawRectangle(&tile_image, 0, 0, 1, 32, ColorBrightness(WHITE, -0.1f));
    ImageDrawRectangle(&tile_image, 31, 0, 1, 32, ColorBrightness(WHITE, -0.1f));
    ImageDrawRectangle(&tile_image, 1, 0, 30, 2, ColorBrightness(WHITE, -0.1f));
    ImageDrawRectangle(&tile_image, 1, 31, 30, 2, ColorBrightness(WHITE, -0.1f));
    Texture2D tile_texture = LoadTextureFromImage(tile_image);
    UnloadImage(tile_image);

    const NPatchInfo tile_n_patch = {(Rectangle) {0, 0, tile_texture.width, tile_texture.height}, 2, 2, 2, 2, NPATCH_NINE_PATCH};

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W)) {
            state.camera.target.y -= GetFrameTime() * speed;
        }
        if (IsKeyDown(KEY_S)) {
            state.camera.target.y += GetFrameTime() * speed;
        }
        if (IsKeyDown(KEY_A)) {
            state.camera.target.x -= GetFrameTime() * speed;
        }
        if (IsKeyDown(KEY_D)) {
            state.camera.target.x += GetFrameTime() * speed;
        }
        
        state.camera.zoom += GetMouseWheelMoveV().y * 0.02f;
        if (state.camera.zoom <= 0.02f) {
            state.camera.zoom = 0.02f;
        }
        speed = 500 / state.camera.zoom;


        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

#ifdef DEBUG
        if (IsKeyDown(KEY_B)) {
            DrawRectangleLinesEx((Rectangle) {WINDOW_WIDTH/4, WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/2}, 5, ColorAlpha(RED, 0.7));
        }
#endif

        BeginMode2D(state.camera);
        for (int region_index = 0; region_index < state.map.region_count; region_index++) {
            const Region region = state.map.regions[region_index];
            for (int area_index = 0; area_index < region.area_count; area_index++) {
                const Area area = region.areas[area_index];
                for (int zone_index = 0; zone_index < area.zone_count; zone_index++) {
                    const Zone zone = area.zones[zone_index];
                    if (!is_zone_on_screen(state.camera, zone)) {
                        continue;
                    }
                    DrawTextureNPatch(tile_texture, tile_n_patch, (Rectangle) {zone.x, zone.y, zone.width, zone.height}, (Vector2) {0, 0}, 0, ZONE_COLORS[zone.type]);
                }
            }
        }
        EndMode2D();
#ifdef DEBUG
        DrawFPS(10, 10);
#endif
        EndDrawing();
    }

    CloseWindow();
    return 0;
}