#include "player.h"
#include "../util/random.h"
#include "../util/common.h"
#include "../map/map.h"
#include <malloc.h>
#include <assert.h>

#define DEFAULT_CAPACITY 8

void player_set_init(PlayerSet* player_set) {
    assert(player_set != NULL);
    player_set->stored = 0;
    player_set->capacity = DEFAULT_CAPACITY;
    player_set->players = (Player**) malloc(sizeof(Player*) * DEFAULT_CAPACITY);
    if (player_set->players == NULL) {
        TraceLog(LOG_FATAL, MEMFAIL"player_set_init");
    }
}

void player_set_destroy(PlayerSet* player_set) {
    assert(player_set != NULL);
    assert(player_set->players != NULL);
    free(player_set->players);
    player_set->players = NULL;
}

void player_set_add(PlayerSet* player_set, Player* player) {
    assert(player_set != NULL);
    assert(player_set->players != NULL);
    assert(player != NULL);
    if (player_set->stored >= player_set->capacity) {
        player_set->capacity *= 1.5;
        Player** players = realloc(player_set->players, sizeof(Player*) * player_set->capacity);
        if (players == NULL) {
            TraceLog(LOG_FATAL, MEMFAIL"player_set_add");
            return;
        }
        player_set->players = players;
    }
    player_set->players[player_set->stored++] = player;
}

void player_set_remove(PlayerSet* player_set, uint16_t index) {
    assert(player_set != NULL);
    assert(index < player_set->capacity);
    player_destroy(player_set->players[index]);
    player_set->players[index] = player_set->players[--player_set->stored];
}

Player* player_init(Area* area, uint8_t hero_type) {
    assert(area != NULL);
    Player* player = (Player*) malloc(sizeof(Player));
    if (player == NULL) {
        TraceLog(LOG_FATAL, MEMFAIL"player_init");
        return NULL;
    }
    player->hero_type = hero_type;
    player->radius = 15;
    player->speed = 150;
    Zone* spawn_zone = area->spawn_zone;
    player->position.x = uniform_random(spawn_zone->x + player->radius, spawn_zone->x + spawn_zone->width - player->radius);
    player->position.y = uniform_random(spawn_zone->y + player->radius, spawn_zone->y + spawn_zone->height - player->radius);
    return player;
}

void player_destroy(Player* player) {
    free(player);
}

void player_update(Player* player) {
}

void player_draw(Player* player) {
    DrawCircleV(player->position, player->radius, GetColor(0x00FF00FF));
}