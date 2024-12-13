#pragma once

#include <stdio.h>
#include <stdint.h>

#define STRING_MAX 64

struct BinaryMap {
    uint8_t spawn_region_length;
    char* spawn_region;
    uint16_t region_count;
    struct BinaryRegion* regions;
};

struct BinaryRegion {
    uint8_t region_name_length;
    char* region_name;
    uint16_t area_count;
    struct BinaryArea* areas;
};

struct BinaryArea {
    int32_t x;
    int32_t y;
    uint16_t zone_count;
    struct BinaryZone* zones;
};

struct BinaryZone {
    uint8_t type;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

void load_map(struct BinaryMap* map, FILE* file);