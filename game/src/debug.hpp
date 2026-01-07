#pragma once

#include <stdint.h>

#define MAX_TIMINGS 180

struct DebugState {
    int64_t tick_times[MAX_TIMINGS];
    int64_t render_times[MAX_TIMINGS];
    int64_t tick_time_sum;
    int64_t render_time_sum;
    uint8_t tick_time_index;
    uint8_t render_time_index;
    uint8_t filled_tick_times : 1;
    uint8_t filled_render_times : 1;
    uint8_t show_timings : 1;

    DebugState();
    void start_timing();
    void end_tick();
    void end_render();
    void draw();
};