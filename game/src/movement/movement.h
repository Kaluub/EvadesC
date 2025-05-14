#pragma once

#include "../map.h"
#include "../entity/enemy.h"

movement_function get_movement_function(uint8_t enemy_type);
void normal_movement(Area* area, Enemy* enemy);
