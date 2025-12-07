#include "movement.h"
#include <math.h>
#include <malloc.h>

typedef struct DasherMovementData {
    float angle;
    float vx;
    float vy;
    float timer;
} DasherMovementData;

void dasher_set_angle(DasherMovementData* data, float angle) {
    if (angle > 2*PI) {
        angle -= 2*PI;
    }
    data->angle = angle;
    data->vx = cosf(angle);
    data->vy = sinf(angle);
}

void dasher_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        DasherMovementData* movement_data = malloc(sizeof(DasherMovementData));
        if (enemy->angle >= 0) {
            dasher_set_angle(movement_data, enemy->angle);
        } else {
            dasher_set_angle(movement_data, uniform_random(0, 2*PI));
        }
        movement_data->timer = 0;
        enemy->movement_data = movement_data;
    }

    DasherMovementData* movement_data = (DasherMovementData*) enemy->movement_data;

    float speed_multiplier;

    movement_data->timer += 1000 * GetFrameTime();
    if (movement_data->timer < 750) {
        // Preparing
        speed_multiplier = 0.2 * (1 - movement_data->timer / 750);
    } else if (movement_data->timer < 3750) {
        // Dashing
        float time = movement_data->timer - 750;
        speed_multiplier = 1 - (time / 3000);
    } else if (movement_data->timer < 4500) {
        speed_multiplier = 0;
    } else {
        movement_data->timer = 0;
        speed_multiplier = 0;
    }

    const float frame_speed = enemy->base_speed * speed_multiplier * GetFrameTime();
    enemy->position.x += movement_data->vx * frame_speed;
    enemy->position.y += movement_data->vy * frame_speed;

    if (enemy->position.x < area->active_zone->x + enemy->radius) {
        enemy->position.x = area->active_zone->x + enemy->radius;
        dasher_set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.x > area->active_zone->x + area->active_zone->width - enemy->radius) {
        enemy->position.x = area->active_zone->x + area->active_zone->width - enemy->radius;
        dasher_set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.y < area->active_zone->y + enemy->radius) {
        enemy->position.y = area->active_zone->y + enemy->radius;
        dasher_set_angle(movement_data, 2*PI - movement_data->angle);
    }
    if (enemy->position.y > area->active_zone->y + area->active_zone->height - enemy->radius) {
        enemy->position.y = area->active_zone->y + area->active_zone->height - enemy->radius;
        dasher_set_angle(movement_data, 2*PI - movement_data->angle);
    }
}