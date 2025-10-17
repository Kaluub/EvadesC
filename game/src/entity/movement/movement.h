#pragma once

#include "../../map/map.h"
#include "../../util/random.h"
#include "../enemy.h"

// Base used for a lot of projectiles.
typedef struct NormalMovementData {
    float angle;
    float vx;
    float vy;
} NormalMovementData;

void set_angle(NormalMovementData* data, float angle);

void set_wall_movement_data(Area* area, Enemy* enemy, int8_t movement_direction);

movement_function get_movement_function(uint8_t enemy_type);
void normal_movement(Area* area, Enemy* enemy);
void wall_movement(Area* area, Enemy* enemy);
void dasher_movement(Area* area, Enemy* enemy);
void oscillating_movement(Area* area, Enemy* enemy);
void teleporting_movement(Area* area, Enemy* enemy);
void star_movement(Area* area, Enemy* enemy);
void icicle_movement(Area* area, Enemy* enemy);