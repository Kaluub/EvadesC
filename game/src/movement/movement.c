#include "movement.h"
#include "../entity/enemy_type.h"

movement_function get_movement_function(uint8_t enemy_type) {
    switch (enemy_type) {
        case ENEMY_TYPE_WALL:
            return wall_movement;
        case ENEMY_TYPE_TELEPORTING:
            return teleporting_movement;
        default:
            return normal_movement;
    }
}