#include "enemy.h"
#include "enemy_type.h"
#include "../movement/movement.h"
#include "../util/common.h"
#include "../circle.h"
#include "raymath.h"
#include <assert.h>
#include <malloc.h>

// EnemySet implementation

void enemy_set_init(EnemySet* enemy_set, uint16_t min_capacity) {
    assert(enemy_set != NULL);
    enemy_set->stored = 0;
    enemy_set->capacity = min_capacity + min_capacity / 4;
    enemy_set->enemies = malloc(sizeof(Enemy*) * enemy_set->capacity);
    if (enemy_set->enemies == NULL) {
        TraceLog(LOG_FATAL, MEMFAIL"enemy_set_init");
    }
}

void enemy_set_add(EnemySet* enemy_set, Enemy* enemy) {
    assert(enemy_set != NULL);
    assert(enemy != NULL);
    if (enemy_set->stored >= enemy_set->capacity) {
        enemy_set->capacity *= 1.5;
        Enemy** enemies = realloc(enemy_set->enemies, sizeof(Enemy*) * enemy_set->capacity);
        if (enemies == NULL) {
            TraceLog(LOG_FATAL, MEMFAIL"enemy_set_add");
            return;
        }
        enemy_set->enemies = enemies;
    }
    enemy_set->enemies[enemy_set->stored++] = enemy;
}

void enemy_set_remove(EnemySet* enemy_set, uint16_t index) {
    assert(enemy_set != NULL);
    assert(index < enemy_set->capacity);
    enemy_destroy(enemy_set->enemies[index]);
    enemy_set->enemies[index] = enemy_set->enemies[--enemy_set->stored];
}

void enemy_set_destroy(EnemySet* enemy_set) {
    assert(enemy_set != NULL);
    assert(enemy_set->enemies != NULL);
    for (int i = 0; i < enemy_set->stored; i++) {
        enemy_destroy(enemy_set->enemies[i]);
    }
    free(enemy_set->enemies);
}

void enemy_set_update(EnemySet* enemy_set, Area* area) {
    for (int i = 0; i < enemy_set->stored; i++) {
        Enemy* enemy = enemy_set->enemies[i];
        enemy->movement_function(area, enemy);
    }
}

void enemy_set_draw(const EnemySet* enemy_set) {
    for (int i = 0; i < enemy_set->stored; i++) {
        Enemy* enemy = enemy_set->enemies[i];
        draw_circle(enemy->position, enemy->radius, enemy->color);
    }
}

// Enemy implementation

Enemy* enemy_init(uint8_t type, Vector2 position, float base_speed, float radius) {
    Enemy* enemy = malloc(sizeof(Enemy));
    if (enemy == NULL) {
        TraceLog(LOG_FATAL, MEMFAIL"enemy_init");
        return NULL;
    }
    enemy->movement_function = get_movement_function(type);
    enemy->movement_data = NULL;
    enemy->position = position;
    enemy->color = GetColor(enemy_colors[type]);
    enemy->base_speed = base_speed;
    enemy->radius = radius;
    enemy->type = type;
    return enemy;
}

void enemy_destroy(Enemy* enemy) {
    assert(enemy != NULL);
    if (enemy->movement_data != NULL) {
        free(enemy->movement_data);
    }
    free(enemy);
}