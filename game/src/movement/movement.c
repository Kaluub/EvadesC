#include "movement.h"

movement_function get_movement_function(uint8_t enemy_type) {
    switch (enemy_type) {
        default:
            return normal_movement;
    }
}