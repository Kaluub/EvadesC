#pragma once

#include <stdio.h>
#include <stdint.h>

#define STRING_MAX 64

typedef struct Map {
    char* spawn_region;
    struct Region* regions;
    uint16_t region_count;
} Map;

typedef struct Region {
    char* region_name;
    struct Area* areas;
    int32_t left_bound;
    int32_t right_bound;
    int32_t top_bound;
    int32_t bottom_bound;
    uint16_t area_count;
} Region;

typedef struct Area {
    struct Zone* zones;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    uint16_t zone_count;
} Area;

typedef struct Zone {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    uint32_t background_color;
    uint8_t type;
} Zone;

void load_map(Map* map, FILE* file);
void destroy_map(Map* map);