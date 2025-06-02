#include "movement.h"
#include <math.h>
#include <malloc.h>

typedef struct TeleportingMovementData {
    float angle;
    float vx;
    float vy;
    float pause_time;
} TeleportingMovementData;

void teleporting_set_angle(TeleportingMovementData* data, float angle) {
    if (angle > 2*PI) {
        angle -= 2*PI;
    }
    data->angle = angle;
    data->vx = cosf(angle);
    data->vy = sinf(angle);
}

void teleporting_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        TeleportingMovementData* movement_data = malloc(sizeof(TeleportingMovementData));
        teleporting_set_angle(movement_data, uniform_random(0, 2*PI));
        movement_data->pause_time = 0;
        enemy->movement_data = movement_data;
    }

    TeleportingMovementData* movement_data = (TeleportingMovementData*) enemy->movement_data;
    const float pause_interval = 1400.0f / (enemy->base_speed / 30.0f);
    if (movement_data->pause_time < pause_interval) {
        movement_data->pause_time += 1000 * GetFrameTime();
        return;
    }

    movement_data->pause_time = 0;
    const float frame_speed = 42;
    enemy->position.x += movement_data->vx * frame_speed;
    enemy->position.y += movement_data->vy * frame_speed;

    if (enemy->position.x < area->active_zone->x + enemy->radius) {
        enemy->position.x = area->active_zone->x + enemy->radius;
        teleporting_set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.x > area->active_zone->x + area->active_zone->width - enemy->radius) {
        enemy->position.x = area->active_zone->x + area->active_zone->width - enemy->radius;
        teleporting_set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.y < area->active_zone->y + enemy->radius) {
        enemy->position.y = area->active_zone->y + enemy->radius;
        teleporting_set_angle(movement_data, 2*PI - movement_data->angle);
    }
    if (enemy->position.y > area->active_zone->y + area->active_zone->height - enemy->radius) {
        enemy->position.y = area->active_zone->y + area->active_zone->height - enemy->radius;
        teleporting_set_angle(movement_data, 2*PI - movement_data->angle);
    }
}