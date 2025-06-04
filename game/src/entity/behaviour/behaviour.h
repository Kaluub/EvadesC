#pragma once

#include "../../map/map.h"
#include "../../util/random.h"
#include "../enemy.h"

behaviour_function get_behaviour_function(uint8_t enemy_type);
void switch_behaviour(Area* area, Enemy* enemy);
void sizing_behaviour(Area* area, Enemy* enemy);
void radiating_bullets_behaviour(Area* area, Enemy* enemy);