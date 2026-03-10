#pragma once

#include "../../map/map.hpp"
#include "../../util/random.hpp"
#include "../enemy.hpp"

class EnemyMovement {
public:
    virtual void update(Area* area, Enemy* enemy) = 0;
};
