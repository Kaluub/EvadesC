#pragma once

#include "../../map/map.hpp"
#include "../../util/random.hpp"
#include "../enemy.hpp"

class EnemyBehaviour {
public:
    virtual void update(Area* area, Enemy* enemy) = 0;
};