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
    uint8_t type;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} Zone;

void load_map(struct Map* map, FILE* file);