#include "enemy.hpp"
#include "enemy_type.hpp"
#include "enemy_effect_type.hpp"
#include "../util/common.h"
#include "../util/random.h"
#include "../circle.h"
#include "raymath.h"
#include <assert.h>
#include <malloc.h>

// EnemySet implementation

extern Shader circle_shader;
extern bool use_circle_shader;

EnemySet::EnemySet(uint16_t min_capacity) {
    stored = 0;
    capacity = min_capacity + min_capacity / 4;
    enemies = (Enemy**) malloc(sizeof(Enemy*) * capacity);
    if (enemies == NULL) {
        TraceLog(LOG_FATAL, MEMFAIL"enemy_set_init");
    }
}

EnemySet::~EnemySet() {
    for (int i = 0; i < stored; i++) {
        enemy_destroy(enemies[i]);
    }
    free(enemies);
}

void EnemySet::add(Enemy* enemy) {
    if (stored >= capacity) {
        capacity *= 1.5;
        Enemy** enemies = (Enemy**) realloc(enemies, sizeof(Enemy*) * capacity);
        if (enemies == NULL) {
            TraceLog(LOG_FATAL, MEMFAIL"enemy_set_add");
            return;
        }
        enemies = enemies;
    }
    enemies[stored++] = enemy;
}

void EnemySet::remove(uint16_t index) {
    enemy_destroy(enemies[index]);
    enemies[index] = enemies[--stored];
}

void EnemySet::update(Area* area) {
    for (int i = 0; i < stored; i++) {
        Enemy* enemy = enemies[i];
        enemy->reset_effects();
        enemy->update(area);

        if (enemy->duration > 0) {
            enemy->duration -= GetFrameTime();
            if (enemy->duration <= 0) {
                enemy->removed = true;
            }
        }

        if (enemy->removed) {
            remove(i--);
        }
    }
}

void EnemySet::draw(float camera_zoom) {
    for (int i = 0; i < stored; i++) {
        Enemy* enemy = enemies[i];
        EnemyEffectConfig effect_config = enemy_effect_configs[enemy->type];
        float radius = enemy->effect_radius;
        if (camera_zoom * radius < 0.1f) {
            continue;
        }
        draw_circle(enemy->position, radius, GetColor(effect_config.color));
    }
    if (use_circle_shader) BeginShaderMode(circle_shader);
    for (int i = 0; i < stored; i++) {
        Enemy* enemy = enemies[i];
        if (camera_zoom * enemy->radius < 0.1f) {
            continue;
        }
        float alpha = (float)enemy->color.a / 255;
        if (enemy->harmless) {
            alpha *= 0.4;
        }
        draw_circle(enemy->position, enemy->radius, ColorAlpha(enemy->color, alpha));
    }
    if (use_circle_shader) EndShaderMode();
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
    enemy->effect_radius = enemy_effect_configs[enemy_type].radius;
    enemy->angle = -1.0f;
    enemy->type = enemy_type;
    enemy->wall_behaviour = WALL_BEHAVIOUR_BOUNCE;
    enemy->harmless = false;
    enemy->removed = false;

    if (spawner->spawner_properties & (1 << MOVE_COUNTER_CLOCKWISE)) {
        set_wall_movement_data(area, enemy, -1);
    }
    if (spawner->spawner_properties & (1 << HAS_X)) {
        enemy->position.x = area->x + (int32_t) spawner->spawn_x;
    }
    if (spawner->spawner_properties & (1 << HAS_Y)) {
        enemy->position.y = area->y + (int32_t) spawner->spawn_y;
    }
    if (spawner->spawner_properties & (1 << HAS_ANGLE)) {
        enemy->angle = DEG2RAD * spawner->angle;
    }
    if (spawner->spawner_properties & (1 << HAS_EFFECT_RADIUS)) {
        enemy->effect_radius = spawner->effect_radius;
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