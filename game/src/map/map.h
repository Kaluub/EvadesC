#pragma once

#include "../entity/enemy.h"
#include <stdio.h>
#include <stdint.h>

enum ComponentProperties {
    HAS_BACKGROUND_COLOR = 0,
    HAS_TEXTURE = 1,
    HAS_NAME = 2,
    HAS_SPAWNER = 3,
    HAS_TRANSLATE = 4,
    USES_PREVIOUS_AREA_DIMENSIONS = 5,
};

enum SpawnerProperties {
    MOVE_COUNTER_CLOCKWISE = 0,
};

enum TextureType {
    TEXTURE_NONE = 0,
    TEXTURE_LEAVES = 1,
};

enum ZoneType {
    ZONE_SAFE = 1,
    ZONE_ACTIVE = 2,
    ZONE_EXIT = 3,
    ZONE_TELEPORT = 4,
    ZONE_VICTORY = 5,
    ZONE_REMOVAL = 6,
    ZONE_DUMMY = 7,
};

typedef struct Map {
    struct Region* regions;
    struct Area* spawn_area;
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
    EnemySet* enemy_set;
    struct Zone* zones;
    struct Zone* active_zone;
    struct Zone* spawn_zone;
    char* area_name;
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
    int32_t translate_x;
    int32_t translate_y;
    uint8_t applies_translate;
    uint8_t type;
    uint8_t texture;
    uint8_t spawner_count;
} Zone;

typedef struct Spawner {
    uint8_t* enemy_types;
    float speed;
    uint16_t count;
    uint16_t radius;
    uint8_t enemy_type_count;
    uint8_t spawner_properties;
} Spawner;

void load_map(Map* map, FILE* file);
void destroy_map(Map* map);