#include "debug.h"
#include "raylib.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX_TIMINGS 180

#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW CLOCK_MONOTONIC
#endif

struct {
    int64_t tick_times[MAX_TIMINGS];
    int64_t render_times[MAX_TIMINGS];
    int64_t tick_time_sum;
    int64_t render_time_sum;
    uint8_t tick_time_index;
    uint8_t render_time_index;
    uint8_t filled_tick_times : 1;
    uint8_t filled_render_times : 1;
    uint8_t show_timings : 1;
} state;

struct timespec start;
struct timespec end;
Font debug_font;

void init_debug_state() {
    state.tick_time_sum = 0;
    state.render_time_sum = 0;
    state.tick_time_index = 0;
    state.render_time_index = 0;
    state.filled_tick_times = 0;
    state.filled_render_times = 0;
    state.show_timings = 0;
    debug_font = GetFontDefault();
}

void timing_start() {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
}

void tick_end() {
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    if (state.filled_tick_times) {
        int64_t old_time = state.tick_times[state.tick_time_index];
        state.tick_time_sum -= old_time;
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        state.tick_times[state.tick_time_index] = new_time;
        state.tick_time_sum += new_time;
        state.tick_time_index = (state.tick_time_index + 1) % MAX_TIMINGS;
    } else {
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        state.tick_times[state.tick_time_index] = new_time;
        state.tick_time_sum += new_time;
        state.tick_time_index++;
        if (state.tick_time_index >= MAX_TIMINGS) {
            state.filled_tick_times = 1;
        }
    }
}

void render_end() {
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    if (state.filled_render_times) {
        int64_t old_time = state.render_times[state.render_time_index];
        state.render_time_sum -= old_time;
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        state.render_times[state.render_time_index] = new_time;
        state.render_time_sum += new_time;
        state.render_time_index = (state.render_time_index + 1) % MAX_TIMINGS;
    } else {
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        state.render_times[state.render_time_index] = new_time;
        state.render_time_sum += new_time;
        state.render_time_index++;
        if (state.render_time_index >= MAX_TIMINGS) {
            state.filled_render_times = 1;
        }
    }
}

void draw_timings() {
    if (IsKeyPressed(KEY_SLASH)) {
        state.show_timings = !state.show_timings;
    }

    if (!state.show_timings) {
        return;
    }
    
    char buff[40] = {0};
    int64_t divisor = (state.filled_tick_times ? MAX_TIMINGS : state.tick_time_index) * 1000000;
    strcpy(buff, TextFormat("tick: %.5fms", (double) state.tick_time_sum / divisor));
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 148, 12}, 16, 1, BLACK);
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 150, 10}, 16, 1, WHITE);
    strcpy(buff, TextFormat("render: %.5fms", (double) state.render_time_sum / divisor));
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 148, 30}, 16, 1, BLACK);
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 150, 28}, 16, 1, WHITE);
}