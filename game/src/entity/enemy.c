#include "enemy.h"
#include "enemy_type.h"
#include "movement/movement.h"
#include "behaviour/behaviour.h"
#include "../util/common.h"
#include "../util/random.h"
#include "../circle.h"
#include "raymath.h"
#include <assert.h>
#include <malloc.h>

// EnemySet implementation

void enemy_set_init(EnemySet** enemy_set, uint16_t min_capacity) {
    assert(enemy_set != NULL);
    EnemySet* new_enemy_set = malloc(sizeof(EnemySet));
    new_enemy_set->stored = 0;
    new_enemy_set->capacity = min_capacity + min_capacity / 4;
    new_enemy_set->enemies = (Enemy**) malloc(sizeof(Enemy*) * new_enemy_set->capacity);
    if (new_enemy_set->enemies == NULL) {
        TraceLog(LOG_FATAL, MEMFAIL"enemy_set_init");
    }
    *enemy_set = new_enemy_set;
}

void enemy_set_destroy(EnemySet* enemy_set) {
    assert(enemy_set != NULL);
    assert(enemy_set->enemies != NULL);
    for (int i = 0; i < enemy_set->stored; i++) {
        enemy_destroy(enemy_set->enemies[i]);
    }
    free(enemy_set->enemies);
    free(enemy_set);
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

void enemy_set_update(EnemySet* enemy_set, Area* area) {
    for (int i = 0; i < enemy_set->stored; i++) {
        Enemy* enemy = enemy_set->enemies[i];
        enemy_reset_effects(enemy);
        enemy->movement_function(area, enemy);
        if (enemy->behaviour_function != NULL) {
            enemy->behaviour_function(area, enemy);
        }

        if (enemy->duration > 0) {
            enemy->duration -= GetFrameTime();
            if (enemy->duration <= 0) {
                enemy->removed = true;
            }
        }

        if (enemy->removed) {
            enemy_set_remove(enemy_set, i--);
        }
    }
}

void enemy_set_draw(const EnemySet* enemy_set, float camera_zoom) {
    for (int i = 0; i < enemy_set->stored; i++) {
        Enemy* enemy = enemy_set->enemies[i];
        if (camera_zoom * enemy->radius < 0.1f) {
            continue;
        }
        float alpha = 1;
        if (enemy->harmless) {
            alpha *= 0.4;
        }
        draw_circle(enemy->position, enemy->radius, ColorAlpha(enemy->color, alpha));
    }
}

// Enemy implementation

Vector2 generate_enemy_position(Zone* zone, Spawner* spawner, uint8_t enemy_type, int spawn_index) {
    switch (enemy_type) {
        case ENEMY_TYPE_WALL:
        case ENEMY_TYPE_WACKY_WALL:
            uint16_t spawn_width = zone->width - 2*spawner->radius;
            uint16_t spawn_height = zone->height - 2*spawner->radius;
            double spawn_length = 2*spawn_width + 2*spawn_height;
            double spawn_spot = spawn_length * spawn_index / spawner->count;
            if (spawner->spawner_properties & (1 << MOVE_COUNTER_CLOCKWISE)) {
                spawn_spot = spawn_length - spawn_spot;
            }
            float x = zone->x + zone->width/2;
            float y = zone->y + spawner->radius;
            if (spawn_spot < 0.5*spawn_width) {
                x -= spawn_spot;
            } else if (spawn_spot < 0.5*spawn_width + spawn_height) {
                x = zone->x + spawner->radius;
                y += spawn_spot - 0.5*spawn_width;
            } else if (spawn_spot < 1.5*spawn_width + spawn_height) {
                x = zone->x + (spawn_spot - spawn_height - 0.5*spawn_width) + spawner->radius;
                y = zone->y + spawn_height + spawner->radius;
            } else if (spawn_spot < 1.5*spawn_width + 2*spawn_height) {
                x = zone->x + spawn_width + spawner->radius;
                y = zone->y + spawn_height + spawner->radius - (spawn_spot - spawn_height - 1.5*spawn_width);
            } else {
                x = zone->x + spawn_width + spawner->radius - (spawn_spot - 1.5*spawn_width - 2*spawn_height);
            }
            return (Vector2) {x, y};
        default:
            return (Vector2) {
                uniform_random(zone->x + spawner->radius, zone->x + zone->width - spawner->radius),
                uniform_random(zone->y + spawner->radius, zone->y + zone->height - spawner->radius)
            };
    }
}

Enemy* enemy_init(Area* area, Zone* zone, Spawner* spawner, int spawn_index) {
    assert(zone != NULL);
    assert(spawner != NULL);
    Enemy* enemy = malloc(sizeof(Enemy));
    if (enemy == NULL) {
        TraceLog(LOG_FATAL, MEMFAIL"enemy_init");
        return NULL;
    }
    uint8_t enemy_type = spawner->enemy_types[discrete_random(0, spawner->enemy_type_count)];
    enemy->movement_function = get_movement_function(enemy_type);
    enemy->movement_data = NULL;
    enemy->behaviour_function = get_behaviour_function(enemy_type);
    enemy->behaviour_data = NULL;
    enemy->position = generate_enemy_position(zone, spawner, enemy_type, spawn_index);
    enemy->color = GetColor(enemy_colors[enemy_type]);
    enemy->base_speed = spawner->speed;
    enemy->radius = spawner->radius;
    enemy->duration = 0;
    enemy->type = enemy_type;
    enemy->wall_behaviour = WALL_BEHAVIOUR_BOUNCE;
    enemy->harmless = false;
    enemy->removed = false;

    if (spawner->spawner_properties & (1 << MOVE_COUNTER_CLOCKWISE)) {
        set_wall_movement_data(area, enemy, -1);
    }

    return enemy;
}

Enemy* enemy_manual_init(movement_function movement_function, Color color) {
    // Used for spawning enemies from other enemies, for example.
    Enemy* enemy = malloc(sizeof(Enemy));
    enemy->movement_function = movement_function;
    enemy->movement_data = NULL;
    enemy->behaviour_function = NULL;
    enemy->behaviour_data = NULL;
    enemy->color = color;
    enemy->base_speed = 0;
    enemy->radius = 0;
    enemy->duration = 0;
    enemy->type = ENEMY_TYPE_NORMAL;
    enemy->wall_behaviour = WALL_BEHAVIOUR_BOUNCE;
    enemy->harmless = false;
    enemy->removed = false;
    return enemy;
}

void enemy_destroy(Enemy* enemy) {
    assert(enemy != NULL);
    if (enemy->movement_data != NULL) {
        free(enemy->movement_data);
    }
    if (enemy->behaviour_data != NULL) {
        free(enemy->behaviour_data);
    }
    free(enemy);
}

void enemy_reset_effects(Enemy* enemy) {
    assert(enemy != NULL);
    enemy->harmless = false;
}