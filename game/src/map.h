#pragma once

#include <stdio.h>
#include <stdint.h>

#define STRING_MAX 64

enum ComponentProperties {
    HAS_BACKGROUND_COLOR = 0,
    HAS_TEXTURE = 1,
    HAS_SPAWNER = 2,
};

enum TextureType {
    TEXTURE_NONE = 0,
    TEXTURE_LEAVES = 1,
};

typedef struct Map {
    char* spawn_region;
    struct Region* regions;
    uint8_t region_count;
} Map;

typedef struct Region {
    char* region_name;
    struct Area* areas;
    uint32_t background_color;
    uint16_t area_count;
    uint8_t texture;
} Region;

typedef struct Area {
    struct Zone* zones;
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t background_color;
    uint8_t zone_count;
    uint8_t texture;
} Area;

typedef struct Zone {
    struct Spawner* spawners;
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t background_color;
    uint8_t type;
    uint8_t texture;
    uint8_t spawner_count;
} Zone;

typedef struct Spawner {
    uint8_t* enemy_types;
    uint32_t count;
    float speed;
    uint8_t enemy_type_count;
} Spawner;

void load_map(Map* map, FILE* file);
void destroy_map(Map* map);