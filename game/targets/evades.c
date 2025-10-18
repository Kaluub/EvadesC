#include "../src/state.h"
#include "../src/debug.h"
#include "../src/circle.h"
#include "../src/util/random.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#ifndef PLATFORM_WEB
#include <pthread.h>
#else
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#define BACKGROUND_COLOR (Color) {0x33, 0x33, 0x33, 0xFF}
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

const Color ZONE_COLORS[] = {
    {0, 0, 0, 255},
    {195, 195, 195, 255},
    {255, 255, 255, 255},
    {255, 244, 108, 255},
    {106, 208, 222, 255},
    {255, 244, 108, 255},
    {255, 249, 186, 255},
    {255, 255, 255, 255},
};

bool is_zone_on_screen(Camera2D camera, const Zone zone) {
#ifdef DEBUG
    if (IsKeyDown(KEY_B)) {
        camera.zoom *= 2;
    }
#endif
    Vector2 top_left = GetWorldToScreen2D((Vector2) {zone.x, zone.y}, camera);
    Vector2 bottom_right = GetWorldToScreen2D((Vector2) {zone.x + zone.width, zone.y + zone.height}, camera);

    return ((bottom_right.x >= 0 && top_left.x <= GetScreenWidth()) && (bottom_right.y >= 0 && top_left.y <= GetScreenHeight()));
}

bool is_rect_on_screen(Camera2D camera, const Rectangle rect) {
    Vector2 top_left = GetWorldToScreen2D((Vector2) {rect.x, rect.y}, camera);
    Vector2 bottom_right = GetWorldToScreen2D((Vector2) {rect.x + rect.width, rect.y + rect.height}, camera);
    return ((bottom_right.x >= 0 && top_left.x <= GetScreenWidth()) && (bottom_right.y >= 0 && top_left.y <= GetScreenHeight()));
}

void reload_map(GameState* state) {
    Map old_map = state->map;
    open_map(state);
    destroy_map(&old_map);
    add_splash_message(&state->splash_messages, "Reloaded map");
}

#ifdef DEBUG
#ifndef PLATFORM_WEB
enum WriteMapState {
    NOT_WRITING,
    WRITING,
    WRITING_SUCCESS,
    WRITING_FAILED,
};

uint8_t write_map_state = NOT_WRITING;
pthread_t writing_thread;
void* write_map(void* _) {
    int result = 0;
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        result = system("WORLD_DIR=~/EvadesClassic/server/maps/definitions python3 maps/packer.py");
    } else {
        result = system("python3 maps/packer.py");
    }
    if (result != 0) {
        write_map_state = WRITING_FAILED;
    } else {
        write_map_state = WRITING_SUCCESS;
    }
    return NULL;
}
#endif
#endif


// Buffers for CTRL measuring.
char measure_x[16];
char measure_y[16];

bool render_translations = false;

static GameState state;

#ifndef PLATFORM_WEB
int current_monitor = 0;
bool capped_framerate = true;
float check_monitor_time = 0.0f;
#endif

float speed = 500.0f;

Texture2D tiles[2];
Texture2D help_texture;

float help_texture_alpha = 1.0f;
float help_texture_fade_time = 10.0f;

void game_tick() {
    timing_start(); // Tick time.
    const float frame_time = GetFrameTime();

#ifndef PLATFORM_WEB
    check_monitor_time -= frame_time;
    if (capped_framerate && check_monitor_time <= 0) {
        check_monitor_time = 0.2f;
        int monitor_check = GetCurrentMonitor();
        if (current_monitor != monitor_check) {
            current_monitor = monitor_check;
            int target_refresh_rate = GetMonitorRefreshRate(current_monitor);
            SetTargetFPS(target_refresh_rate);
            add_splash_message(&state.splash_messages, TextFormat("Target framerate adjusted to %d", target_refresh_rate));
        }
    }

    if (IsKeyPressed(KEY_F7)) {
        capped_framerate = !capped_framerate;
        if (!capped_framerate) {
            SetTargetFPS(0);
        } else {
            SetTargetFPS(GetMonitorRefreshRate(current_monitor));
        }
        const char* options[] = {"uncapped", "capped"};
        add_splash_message(&state.splash_messages, TextFormat("Framerate is now %s.", options[capped_framerate]));
    }
#endif

    if (help_texture_fade_time > 0.0f) {
        help_texture_fade_time -= frame_time;
    } else if (help_texture_alpha > 0.0f) {
        // Help texture fades over 0.75s
        help_texture_alpha -= frame_time / (3.0f/4);
    }

    if (IsKeyPressed(KEY_H)) {
        if (help_texture_alpha < 1.0f) {
            help_texture_alpha = 1.0f;
            help_texture_fade_time = 15.0f;
        } else {
            help_texture_alpha = 0.0f;
            help_texture_fade_time = 0.0f;
        }
    }

    if (IsKeyPressed(KEY_T)) {
        render_translations = !render_translations;
        const char* options[] = {"no longer", "now"};
        add_splash_message(&state.splash_messages, TextFormat("Zone translations are %s displayed.", options[render_translations]));
    }

    state.camera.offset = (Vector2) {GetScreenWidth()/2, GetScreenHeight()/2};
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
    } else if (IsKeyDown(KEY_SPACE)) {
        speed *= 2;
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
    
    int gesture = GetGestureDetected();
    if (gesture == GESTURE_PINCH_OUT) {
        state.camera.zoom += 2 * frame_time * state.camera.zoom;
    } else if (gesture == GESTURE_PINCH_IN) {
        state.camera.zoom -= 2 * frame_time * state.camera.zoom;
    } else if (GetTouchPointCount() > 0) {
        Vector2 touch_position = Vector2Scale(Vector2Add(GetTouchPosition(0), (Vector2) {-GetScreenWidth()/2, -GetScreenHeight()/2}), 1.0f/fminf(GetScreenHeight()/3, GetScreenWidth()/3));
        state.camera.target = Vector2Add(state.camera.target, Vector2Scale(touch_position, GetFrameTime() * speed));
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
        DrawRectangleLinesEx((Rectangle) {GetScreenWidth()/4, GetScreenHeight()/4, GetScreenWidth()/2, GetScreenHeight()/2}, 5, ColorAlpha(RED, 0.7));
    }

    if (IsKeyPressed(KEY_L)) {
#ifndef PLATFORM_WEB
        if (IsKeyDown(KEY_LEFT_SHIFT) && write_map_state == NOT_WRITING) {
            // Requires dev environment.
            write_map_state = WRITING;
            add_splash_message(&state.splash_messages, "Writing world.bin...");
            pthread_create(&writing_thread, NULL, write_map, NULL);
        } else {
            reload_map(&state);
        }
    }
    if (write_map_state == WRITING_SUCCESS) {
        reload_map(&state);
        write_map_state = NOT_WRITING;
    }
    if (write_map_state == WRITING_FAILED) {
        add_splash_message(&state.splash_messages, "Writing failed, see console for error. Continuing with currently loaded world.");
        write_map_state = NOT_WRITING;
    }
#else
        reload_map(&state);
    }
#endif
#endif

    BeginMode2D(state.camera);
    Texture2D tile_texture;
    for (int region_index = 0; region_index < state.map.region_count; region_index++) {
        Region region = state.map.regions[region_index];
        for (int area_index = 0; area_index < region.area_count; area_index++) {
            Area area = region.areas[area_index];
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
            if (area_screen_location.x > GetScreenWidth() || area_screen_edge.x < 0
                || area_screen_location.y > GetScreenHeight() || area_screen_edge.y < 0) {
                continue;
            }
            for (int zone_index = 0; zone_index < area.zone_count; zone_index++) {
                Zone zone = area.zones[zone_index];
                if (!is_zone_on_screen(state.camera, zone)) {
                    continue;
                }
                tile_texture = tiles[zone.texture];
                if (state.camera.zoom > 0.4 || zone.texture != TEXTURE_NONE) {
                    for (int x_off = 0; x_off <= (int)zone.width; x_off += tile_texture.width) {
                        for (int y_off = 0; y_off <= (int)zone.height; y_off += tile_texture.height) {
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

            if (IsKeyDown(KEY_TAB)) {
                DrawText(
                    TextFormat(
                        "Area %d%s\n%d x %d",
                        area_index + 1,
                        area.area_name != NULL
                        ? TextFormat(" (%s)", area.area_name)
                        : "",
                        area.width,
                        area.height
                    ), area.x + 10, area.y, 64, BLACK);
            }
            enemy_set_update(area.enemy_set, region.areas + area_index);
            enemy_set_draw(area.enemy_set, state.camera.zoom);
        }
    }

    if (render_translations) {
        for (int region_index = 0; region_index < state.map.region_count; region_index++) {
            Region region = state.map.regions[region_index];
            for (int area_index = 0; area_index < region.area_count; area_index++) {
                Area area = region.areas[area_index];
                for (int zone_index = 0; zone_index < area.zone_count; zone_index++) {
                    Zone zone = area.zones[zone_index];
                    if (zone.applies_translate) {
                        Rectangle zone_rect = {zone.x, zone.y, zone.width, zone.height};
                        Rectangle zone_translate_rect = {zone.x + zone.translate_x, zone.y + zone.translate_y, zone.width, zone.height};
                        bool zone_rect_visible = is_rect_on_screen(state.camera, zone_rect);
                        bool zone_translate_rect_visible = is_rect_on_screen(state.camera, zone_translate_rect);
                        if (zone_rect_visible) {
                            DrawRectanglePro(zone_rect, (Vector2) {0, 0}, 0.0f, ColorAlpha(BLUE, 0.2f));
                            DrawRectangleLinesEx(zone_rect, 2.0f, BLUE);
                        }
                        if (zone_translate_rect_visible) {
                            DrawRectanglePro(zone_translate_rect, (Vector2) {0, 0}, 0.0f, ColorAlpha(PINK, 0.2f));
                            DrawRectangleLinesEx(zone_translate_rect, 2.0f, PINK);
                        }
                        Vector2 line_start = {zone.x + zone.width/2, zone.y + zone.height/2};
                        Vector2 line_end = {line_start.x + zone.translate_x, line_start.y + zone.translate_y};
                        DrawLineEx(line_start, line_end, 4.0f, (Color) {255, 0, 255, 255});
                    }
                }
            }
        }
    }

    bool draw_measure_x = false;
    bool draw_measure_y = false;
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        if (closest_area_left != NULL) {
            DrawRectangle(closest_area_left->x, closest_area_left->y, closest_area_left->width, closest_area_left->height, (Color) {255, 0, 255, 128});
            if (closest_area_right != NULL) {
                DrawRectangle(closest_area_right->x, closest_area_right->y, closest_area_right->width, closest_area_right->height, (Color) {0, 255, 0, 128});
                DrawLine(closest_area_left->x + closest_area_left->width, mouse_over.y, closest_area_right->x, mouse_over.y, RED);
                TextCopy(measure_x, TextFormat("%d", closest_area_right->x - closest_area_left->x - closest_area_left->width));
                draw_measure_x = true;
            } else {
                DrawLine(closest_area_left->x + closest_area_left->width, mouse_over.y, mouse_over.x, mouse_over.y, RED);
                TextCopy(measure_x, TextFormat("%.0f", mouse_over.x - closest_area_left->x - closest_area_left->width));
                draw_measure_x = true;
            }
        } else if (closest_area_right != NULL) {
            DrawRectangle(closest_area_right->x, closest_area_right->y, closest_area_right->width, closest_area_right->height, (Color) {0, 255, 0, 128});
            DrawLine(mouse_over.x, mouse_over.y, closest_area_right->x, mouse_over.y, RED);
            TextCopy(measure_x, TextFormat("%.0f", closest_area_right->x - mouse_over.x));
            draw_measure_x = true;
        }

        if (closest_area_top != NULL) {
            DrawRectangle(closest_area_top->x, closest_area_top->y, closest_area_top->width, closest_area_top->height, (Color) {255, 0, 0, 128});
            if (closest_area_bottom != NULL) {
                DrawRectangle(closest_area_bottom->x, closest_area_bottom->y, closest_area_bottom->width, closest_area_bottom->height, (Color) {0, 0, 255, 128});
                DrawLine(mouse_over.x, closest_area_top->y + closest_area_top->height, mouse_over.x, closest_area_bottom->y, BLUE);
                TextCopy(measure_y, TextFormat("%d", closest_area_bottom->y - closest_area_top->y - closest_area_top->height));
                draw_measure_y = true;
            } else {
                DrawLine(mouse_over.x, closest_area_top->y + closest_area_top->height, mouse_over.x, mouse_over.y, BLUE);
                TextCopy(measure_y, TextFormat("%.0f", mouse_over.y - closest_area_top->y - closest_area_top->height));
                draw_measure_y = true;
            }
        } else if (closest_area_bottom != NULL) {
            DrawRectangle(closest_area_bottom->x, closest_area_bottom->y, closest_area_bottom->width, closest_area_bottom->height, (Color) {0, 0, 255, 128});
            DrawLine(mouse_over.x, mouse_over.y, mouse_over.x, closest_area_bottom->y, BLUE);
            TextCopy(measure_y, TextFormat("%.0f", closest_area_bottom->y - mouse_over.y));
            draw_measure_y = true;
        }
    }

    EndMode2D();
    Vector2 mouse_pos = GetMousePosition();
    if (draw_measure_x) {
        DrawText(measure_x, mouse_pos.x + 17, mouse_pos.y - 15, 16, BLACK);
        DrawText(measure_x, mouse_pos.x + 16, mouse_pos.y - 16, 16, RED);
    }
    if (draw_measure_y) {
        DrawText(measure_y, mouse_pos.x + 17, mouse_pos.y + 1, 16, BLACK);
        DrawText(measure_y, mouse_pos.x + 16, mouse_pos.y, 16, BLUE);
    }
#ifdef DEBUG
    const char* text = TextFormat("Debug build. FPS: %d", GetFPS());
    DrawText(text, 12, 12, 20, BLACK);
    DrawText(text, 10, 10, 20, LIME);
#endif
    process_splash_messages(&state.splash_messages);
    if (help_texture_alpha > 0) {
        float help_scale = fminf(1.0, (0.5 * GetScreenHeight()) / (float)help_texture.height);
        DrawTextureEx(help_texture, (Vector2) {2, (GetScreenHeight() - help_texture.height * help_scale)/2}, 0, help_scale, ColorAlpha(WHITE, help_texture_alpha));
    }
    render_end();
    draw_timings();
    EndDrawing();
}

#ifdef PLATFORM_WEB
EM_BOOL web_resize(int ev, const EmscriptenUiEvent* e, void* data) {
    SetWindowSize(e->windowInnerWidth, e->windowInnerHeight);
    return EM_FALSE;
}

void resize_game(int width, int height) {
    SetWindowSize(width, height);
}
#endif

int main() {
    init_random();
    open_map(&state);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Evades");
#ifndef PLATFORM_WEB
    current_monitor = GetCurrentMonitor();
    SetTargetFPS(GetMonitorRefreshRate(current_monitor));
#endif

    state.camera.offset = (Vector2) {GetScreenWidth()/2, GetScreenHeight()/2};
    state.camera.rotation = 0;
    state.camera.target = (Vector2) {0, 0};
    state.camera.zoom = 1;
    tiles[TEXTURE_NONE] = LoadTexture("assets/tile.png");
    SetTextureFilter(tiles[TEXTURE_NONE], TEXTURE_FILTER_ANISOTROPIC_16X);
    GenTextureMipmaps(&tiles[TEXTURE_NONE]);
    tiles[TEXTURE_LEAVES] = LoadTexture("assets/leaves.png");
    SetTextureFilter(tiles[TEXTURE_LEAVES], TEXTURE_FILTER_ANISOTROPIC_16X);
    GenTextureMipmaps(&tiles[TEXTURE_LEAVES]);

    help_texture = LoadTexture("assets/help.png");
    SetTextureFilter(help_texture, TEXTURE_FILTER_ANISOTROPIC_16X);

    init_debug_state();
    init_splash_messages(&state.splash_messages);
    init_circle_texture();

#ifndef PLATFORM_WEB
    while (!WindowShouldClose()) {
        game_tick();
    }
#else
    emscripten_run_script("js_resize()");
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, 0, web_resize);
    emscripten_set_main_loop(game_tick, 0, 1);
#endif

    CloseWindow();
    destroy_map(&state.map);
    return EXIT_SUCCESS;
}