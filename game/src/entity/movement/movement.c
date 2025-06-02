#include "movement.h"
#include "../enemy_type.h"

movement_function get_movement_function(uint8_t enemy_type) {
    switch (enemy_type) {
        case ENEMY_TYPE_WALL:
            return wall_movement;
        case ENEMY_TYPE_DASHER:
        case ENEMY_TYPE_DASHER_SWITCH:
            return dasher_movement;
        case ENEMY_TYPE_OSCILLATING:
        case ENEMY_TYPE_OSCILLATING_SWITCH:
            return oscillating_movement;
        case ENEMY_TYPE_TELEPORTING:
            return teleporting_movement;
        case ENEMY_TYPE_STAR:
            return star_movement;
        case ENEMY_TYPE_ICICLE:
            return icicle_movement;
        default:
            return normal_movement;
    }
}