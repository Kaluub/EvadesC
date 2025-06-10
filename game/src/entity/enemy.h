#pragma once

#include "raylib.h"
#include <stdint.h>

// Enemy movement is function based.
// If it requires extra data, eg. an angle or timer, it can
// use movement_data. It is freed when enemy_destroy is called.
// Do not use any extra dynamic allocation in movement_data.

// Enemy behaviour is optional and used for non-movement behaviour, eg. switch, sizing.
// Same as above applies for behaviour_data.

typedef struct Area Area;
typedef struct Enemy Enemy;
typedef struct Zone Zone;
typedef struct Spawner Spawner;
typedef void (*movement_function)(Area* area, Enemy* enemy);
typedef void (*behaviour_function)(Area* area, Enemy* enemy);

enum WallBehaviour {
    WALL_BEHAVIOUR_BOUNCE,
    WALL_BEHAVIOUR_REMOVE,
};

typedef struct Enemy {
    movement_function movement_function;
    void* movement_data;
    behaviour_function behaviour_function;
    void* behaviour_data;
    Vector2 position;
    Color color;
    float base_speed;
    float radius;
    float duration;
    uint8_t type;
    uint8_t wall_behaviour;
    bool harmless : 1;
    bool removed : 1;
} Enemy;

typedef struct EnemySet {
    Enemy** enemies;
    uint16_t stored;
    uint16_t capacity;
} EnemySet;

void enemy_set_init(EnemySet** enemy_set, uint16_t min_capacity);
void enemy_set_destroy(EnemySet* enemy_set);
void enemy_set_add(EnemySet* enemy_set, Enemy* enemy);
void enemy_set_remove(EnemySet* enemy_set, uint16_t index);
void enemy_set_update(EnemySet* enemy_set, Area* area);
void enemy_set_draw(const EnemySet* enemy_set, float camera_zoom);

Enemy* enemy_init(Zone* zone, Spawner* spawner, int spawn_index);
Enemy* enemy_manual_init(movement_function movement_function, Color color);
void enemy_destroy(Enemy* enemy);
void enemy_reset_effects(Enemy* enemy);