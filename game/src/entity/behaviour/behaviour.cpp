#include "behaviour.hpp"
#include "../enemy_type.hpp"

behaviour_function get_behaviour_function(uint8_t enemy_type) {
    switch (enemy_type) {
        case ENEMY_TYPE_SWITCH:
        case ENEMY_TYPE_WAVY_SWITCH:
        case ENEMY_TYPE_PENNY_SWITCH:
        case ENEMY_TYPE_DASHER_SWITCH:
        case ENEMY_TYPE_DORITO_SWITCH:
        case ENEMY_TYPE_HOMING_SWITCH:
        case ENEMY_TYPE_SPIRAL_SWITCH:
        case ENEMY_TYPE_ZIGZAG_SWITCH:
        case ENEMY_TYPE_ZONING_SWITCH:
        case ENEMY_TYPE_INFINITY_SWITCH:
        case ENEMY_TYPE_OSCILLATING_SWITCH:
        case ENEMY_TYPE_CONFECTIONER_SWITCH:
            return switch_behaviour;
        case ENEMY_TYPE_SIZING:
            return sizing_behaviour;
        case ENEMY_TYPE_RADIATING_BULLETS:
            return radiating_bullets_behaviour;
        default:
            return NULL;
    }
}