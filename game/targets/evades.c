#include "../src/state.h"
#include <stdlib.h>
#include <time.h>

#define BACKGROUND_COLOR (Color) {0x33, 0x33, 0x33, 0xFF}
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

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
        camera.zoom *= 2;
    }
#endif
    Vector2 top_left = GetWorldToScreen2D((Vector2) {zone.x, zone.y}, camera);
    Vector2 bottom_right = GetWorldToScreen2D((Vector2) {zone.x + zone.width, zone.y + zone.height}, camera);

    return ((bottom_right.x >= 0 && top_left.x <= WINDOW_WIDTH) && (bottom_right.y >= 0 && top_left.y <= WINDOW_HEIGHT));
}

int main() {
    GameState state;
    open_map(&state);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Evades");
    int current_monitor = GetCurrentMonitor();
    SetTargetFPS(GetMonitorRefreshRate(current_monitor));

    state.camera.offset = (Vector2) {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    state.camera.rotation = 0;
    state.camera.target = (Vector2) {0, 0};
    state.camera.zoom = 1;

    float speed = 500 / state.camera.zoom;

    Texture2D tile_texture = LoadTexture("assets/tile.png");
    SetTextureWrap(tile_texture, TEXTURE_WRAP_REPEAT);

    while (!WindowShouldClose()) {
        if (current_monitor != GetCurrentMonitor()) {
            current_monitor = GetCurrentMonitor();
            SetTargetFPS(GetMonitorRefreshRate(current_monitor));
        }

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

        if (IsKeyPressed(KEY_L)) {
            Map old_map = state.map;
            open_map(&state);
            destroy_map(&old_map);
        }
#endif

        BeginMode2D(state.camera);
        for (int region_index = 0; region_index < state.map.region_count; region_index++) {
            const Region region = state.map.regions[region_index];
            for (int area_index = 0; area_index < region.area_count; area_index++) {
                const Area area = region.areas[area_index];
                Vector2 area_screen_location = GetWorldToScreen2D((Vector2) {area.x, area.y}, state.camera);
                Vector2 area_screen_edge = GetWorldToScreen2D((Vector2) {area.x + area.width, area.y + area.height}, state.camera);
                if (area_screen_location.x > WINDOW_WIDTH || area_screen_edge.x < 0
                    || area_screen_location.y > WINDOW_HEIGHT || area_screen_edge.y < 0) {
                    continue;
                }
                for (int zone_index = 0; zone_index < area.zone_count; zone_index++) {
                    const Zone zone = area.zones[zone_index];
                    if (!is_zone_on_screen(state.camera, zone)) {
                        continue;
                    }
                    DrawTexturePro(tile_texture, (Rectangle) {0, 0, tile_texture.width, tile_texture.height}, (Rectangle) {zone.x, zone.y, zone.width, zone.height}, (Vector2) {0}, 0, ZONE_COLORS[zone.type]);
                    if (zone.background_color != 0) {
                        DrawRectangle(zone.x, zone.y, zone.width, zone.height, GetColor(zone.background_color));
                    }
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
    destroy_map(&state.map);
    return 0;
}