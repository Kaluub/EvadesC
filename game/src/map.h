#pragma once

#include <stdio.h>
#include <stdint.h>

#define STRING_MAX 64

typedef struct Map {
    uint8_t spawn_region_length;
    char* spawn_region;
    uint16_t region_count;
    struct Region* regions;
} Map;

typedef struct Region {
    uint8_t region_name_length;
    char* region_name;
    uint16_t area_count;
    struct Area* areas;
} Region;

typedef struct Area {
    int32_t x;
    int32_t y;
    uint16_t zone_count;
    struct Zone* zones;
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