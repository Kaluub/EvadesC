#pragma once

#include "raylib.h"
#include <stdint.h>

typedef struct Area Area;

typedef struct Player {
    Vector2 position;
    float radius;
    float speed;
    uint8_t hero_type;
} Player;

typedef struct PlayerSet {
    Player** players;
    uint16_t stored;
    uint16_t capacity;
} PlayerSet;

void player_set_init(PlayerSet* player_set);
void player_set_destroy(PlayerSet* player_set);
void player_set_add(PlayerSet* player_set, Player* player);
void player_set_remove(PlayerSet* player_set, uint16_t index);

Player* player_init(Area* area, uint8_t hero_type);
void player_destroy(Player* player);
void player_update(Player* player);
void player_draw(Player* player);