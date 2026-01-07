#include "debug.hpp"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

struct timespec start;
struct timespec end;
Font debug_font;

DebugState::DebugState() {
    tick_time_sum = 0;
    render_time_sum = 0;
    tick_time_index = 0;
    render_time_index = 0;
    filled_tick_times = 0;
    filled_render_times = 0;
    show_timings = 0;
    debug_font = GetFontDefault();
}

void DebugState::start_timing() {
    clock_gettime(CLOCK_MONOTONIC, &start);
}

void DebugState::end_tick() {
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (filled_tick_times) {
        int64_t old_time = tick_times[tick_time_index];
        tick_time_sum -= old_time;
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        tick_times[tick_time_index] = new_time;
        tick_time_sum += new_time;
        tick_time_index = (tick_time_index + 1) % MAX_TIMINGS;
    } else {
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        tick_times[tick_time_index] = new_time;
        tick_time_sum += new_time;
        tick_time_index++;
        if (tick_time_index >= MAX_TIMINGS) {
            filled_tick_times = 1;
            tick_time_index--;
        }
    }
}

void DebugState::end_render() {
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (filled_render_times) {
        int64_t old_time = render_times[render_time_index];
        render_time_sum -= old_time;
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        render_times[render_time_index] = new_time;
        render_time_sum += new_time;
        render_time_index = (render_time_index + 1) % MAX_TIMINGS;
    } else {
        int64_t new_time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        render_times[render_time_index] = new_time;
        render_time_sum += new_time;
        render_time_index++;
        if (render_time_index >= MAX_TIMINGS) {
            filled_render_times = 1;
            render_time_index--;
        }
    }
}

void DebugState::draw() {
    if (IsKeyPressed(KEY_SLASH)) {
        show_timings = !show_timings;
    }

    if (!show_timings) {
        return;
    }
    
    char buff[40] = {0};
    int64_t divisor = (filled_tick_times ? MAX_TIMINGS : tick_time_index) * 1e6;
    strcpy(buff, TextFormat("tick: %.5fms", (double) tick_time_sum / divisor));
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 148, 12}, 16, 1, BLACK);
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 150, 10}, 16, 1, WHITE);
    strcpy(buff, TextFormat("render: %.5fms", (double) render_time_sum / divisor));
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 148, 30}, 16, 1, BLACK);
    DrawTextEx(debug_font, buff, (Vector2) {GetScreenWidth() - 150, 28}, 16, 1, WHITE);
}