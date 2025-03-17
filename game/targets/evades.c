#include "../src/state.h"
#include "../src/debug.h"
#include <math.h>
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

    Texture2D tiles[2];
    tiles[TEXTURE_NONE] = LoadTexture("assets/tile.png");
    SetTextureFilter(tiles[TEXTURE_NONE], TEXTURE_FILTER_ANISOTROPIC_16X);
    tiles[TEXTURE_LEAVES] = LoadTexture("assets/leaves.png");
    SetTextureFilter(tiles[TEXTURE_LEAVES], TEXTURE_FILTER_ANISOTROPIC_16X);

    init_debug_state();

    while (!WindowShouldClose()) {
        timing_start(); // Tick time.
        if (current_monitor != GetCurrentMonitor()) {
            current_monitor = GetCurrentMonitor();
            SetTargetFPS(GetMonitorRefreshRate(current_monitor));
        }

        const float frame_time = GetFrameTime();

        state.camera.zoom += GetMouseWheelMoveV().y * 0.05f * state.camera.zoom;

        if (IsKeyDown(KEY_I)) {
            state.camera.zoom += 2 * frame_time * state.camera.zoom;
        }
        if (IsKeyDown(KEY_O)) {
            state.camera.zoom -= 2 * frame_time * state.camera.zoom;
        }

        if (state.camera.zoom < 0.005f) {
            state.camera.zoom = 0.005f;
        }
        if (state.camera.zoom > 25.0f) {
            state.camera.zoom = 25.0f;
        }
        speed = fmaxf(500 / state.camera.zoom, 10);

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            speed *= 0.5;
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

        Vector2 mouse_over = GetScreenToWorld2D(GetMousePosition(), state.camera);
        Area* closest_area_left = NULL;
        Area* closest_area_top = NULL;
        Area* closest_area_right = NULL;
        Area* closest_area_bottom = NULL;

        tick_end();
        timing_start(); // Render time.

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
        Texture2D tile_texture;
        for (int region_index = 0; region_index < state.map.region_count; region_index++) {
            const Region region = state.map.regions[region_index];
            for (int area_index = 0; area_index < region.area_count; area_index++) {
                const Area area = region.areas[area_index];
                // Calculate closest area bounds
                if (area.y <= mouse_over.y && mouse_over.y <= area.y + area.height) {
                    // Area is aligned with mouse on the y-axis (for left/right)
                    if (mouse_over.x > area.x + area.width &&
                        (closest_area_left == NULL || area.x + area.width > closest_area_left->x + closest_area_left->width)) {
                        closest_area_left = region.areas + area_index;
                    }
                    if (mouse_over.x < area.x &&
                        (closest_area_right == NULL || area.x < closest_area_right->x)) {
                        closest_area_right = region.areas + area_index;
                    }
                }

                if (area.x <= mouse_over.x && mouse_over.x <= area.x + area.width) {
                    // Area is aligned with mouse on the x-axis (for top/bottom)
                    if (mouse_over.y > area.y + area.height &&
                        (closest_area_top == NULL || area.y + area.height > closest_area_top->y + closest_area_top->height)) {
                        closest_area_top = region.areas + area_index;
                    }
                    if (mouse_over.y < area.y &&
                        (closest_area_bottom == NULL || area.y < closest_area_bottom->y)) {
                        closest_area_bottom = region.areas + area_index;
                    }
                }

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
                    tile_texture = tiles[zone.texture];
                    if (state.camera.zoom > 0.4 || zone.texture != TEXTURE_NONE) {
                        for (int x_off = 0; x_off <= zone.width; x_off += tile_texture.width) {
                            for (int y_off = 0; y_off <= zone.height; y_off += tile_texture.height) {
                                DrawTexturePro(
                                    tile_texture,
                                    (Rectangle) {0, 0, fmin(tile_texture.width, zone.width - x_off), fmin(tile_texture.height, zone.height - y_off)},
                                    (Rectangle) {zone.x + x_off, zone.y + y_off, fmin(tile_texture.width, zone.width - x_off), fmin(tile_texture.height, zone.height - y_off)},
                                    (Vector2) {0}, 0, ZONE_COLORS[zone.type]
                                );
                            }
                        }
                    } else {
                        DrawRectangle(zone.x, zone.y, zone.width, zone.height, ZONE_COLORS[zone.type]);
                    }
                    if (zone.background_color != 0) {
                        DrawRectangle(zone.x, zone.y, zone.width, zone.height, GetColor(zone.background_color));
                    }
                }

                DrawText(TextFormat("%d x %d", area.width, area.height), area.x + 10, area.y, 96, BLACK);
            }
        }

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            const int scaled_factor = 16 / state.camera.zoom;
            if (closest_area_left != NULL) {
                DrawRectangle(closest_area_left->x, closest_area_left->y, closest_area_left->width, closest_area_left->height, (Color) {255, 0, 255, 128});
                if (closest_area_right != NULL) {
                    DrawRectangle(closest_area_right->x, closest_area_right->y, closest_area_right->width, closest_area_right->height, (Color) {0, 255, 0, 128});
                    DrawLine(closest_area_left->x + closest_area_left->width, mouse_over.y, closest_area_right->x, mouse_over.y, RED);
                    DrawText(TextFormat("%d", closest_area_right->x - closest_area_left->x - closest_area_left->width), mouse_over.x + scaled_factor, mouse_over.y - scaled_factor, scaled_factor, RED);
                } else {
                    DrawLine(closest_area_left->x + closest_area_left->width, mouse_over.y, mouse_over.x, mouse_over.y, RED);
                    DrawText(TextFormat("%.0f", mouse_over.x - closest_area_left->x - closest_area_left->width), mouse_over.x + scaled_factor, mouse_over.y - scaled_factor, scaled_factor, RED);
                }
            } else if (closest_area_right != NULL) {
                DrawRectangle(closest_area_right->x, closest_area_right->y, closest_area_right->width, closest_area_right->height, (Color) {0, 255, 0, 128});
                DrawLine(mouse_over.x, mouse_over.y, closest_area_right->x, mouse_over.y, RED);
                DrawText(TextFormat("%.0f", closest_area_right->x - mouse_over.x), mouse_over.x + scaled_factor, mouse_over.y - scaled_factor, scaled_factor, RED);
            }

            if (closest_area_top != NULL) {
                DrawRectangle(closest_area_top->x, closest_area_top->y, closest_area_top->width, closest_area_top->height, (Color) {255, 0, 0, 128});
                if (closest_area_bottom != NULL) {
                    DrawRectangle(closest_area_bottom->x, closest_area_bottom->y, closest_area_bottom->width, closest_area_bottom->height, (Color) {0, 0, 255, 128});
                    DrawLine(mouse_over.x, closest_area_top->y + closest_area_top->height, mouse_over.x, closest_area_bottom->y, BLUE);
                    DrawText(TextFormat("%d", closest_area_bottom->y - closest_area_top->y - closest_area_top->height), mouse_over.x + scaled_factor, mouse_over.y, scaled_factor, BLUE);
                } else {
                    DrawLine(mouse_over.x, closest_area_top->y + closest_area_top->height, mouse_over.x, mouse_over.y, BLUE);
                    DrawText(TextFormat("%.0f", mouse_over.y - closest_area_top->y - closest_area_top->height), mouse_over.x + scaled_factor, mouse_over.y, scaled_factor, BLUE);
                }
            } else if (closest_area_bottom != NULL) {
                DrawRectangle(closest_area_bottom->x, closest_area_bottom->y, closest_area_bottom->width, closest_area_bottom->height, (Color) {0, 0, 255, 128});
                DrawLine(mouse_over.x, mouse_over.y, mouse_over.x, closest_area_bottom->y, BLUE);
                DrawText(TextFormat("%.0f", closest_area_bottom->y - mouse_over.y), mouse_over.x + scaled_factor, mouse_over.y, scaled_factor, BLUE);
            }
        }

        if (IsKeyDown(KEY_V)) {
            // Shader circle drawing performance test (TODO)
            for (int i = 0; i < 10000; i += 1) {
                DrawCircle(500 * i, 500 * i, 75, RED);
            }
        }

        EndMode2D();
#ifdef DEBUG
        DrawFPS(10, 10);
#endif
        render_end();
        draw_timings();
        EndDrawing();
    }

    CloseWindow();
    destroy_map(&state.map);
    return 0;
}