#pragma once

#include "raylib.h"
#include <stdint.h>

class Area;
class Zone;
class Spawner;

class Enemy {
public:
    Vector2 position;
    Color color;
    float base_speed;
    float radius;
    float duration;
    float effect_radius;
    float angle;
    uint8_t type;
    bool harmless : 1;
    bool removed : 1;

    Enemy(Area* area, Zone* zone, Spawner* spawner, int spawn_index);
    void update(Area* area);
    void reset_effects();
};

class EnemySet {
public:
    Enemy** enemies;
    uint16_t stored;
    uint16_t capacity;

    EnemySet(uint16_t min_capacity);
    ~EnemySet();
    void add(Enemy* enemy);
    void remove(uint16_t index);
    void update(Area* area);
    void draw(float camera_zoom);
};