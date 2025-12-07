#include "movement.h"
#include <math.h>
#include <malloc.h>

typedef struct OscillatingMovementData {
    float angle;
    float vx;
    float vy;
    float switch_time;
} OscillatingMovementData;

void oscillating_set_angle(OscillatingMovementData* data, float angle) {
    if (angle > 2*PI) {
        angle -= 2*PI;
    }
    data->angle = angle;
    data->vx = cosf(angle);
    data->vy = sinf(angle);
}

void oscillating_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        OscillatingMovementData* movement_data = malloc(sizeof(OscillatingMovementData));
        if (enemy->angle >= 0) {
            oscillating_set_angle(movement_data, enemy->angle);
        } else {
            oscillating_set_angle(movement_data, uniform_random(0, 2*PI));
        }
        movement_data->switch_time = uniform_random(0, 1000);
        enemy->movement_data = movement_data;
    }

    OscillatingMovementData* movement_data = (OscillatingMovementData*) enemy->movement_data;

    movement_data->switch_time -= 1000 * GetFrameTime();
    if (movement_data->switch_time <= 0) {
        movement_data->switch_time = 1000;
        oscillating_set_angle(movement_data, movement_data->angle + PI);
    }

    const float frame_speed = enemy->base_speed * GetFrameTime();
    enemy->position.x += movement_data->vx * frame_speed;
    enemy->position.y += movement_data->vy * frame_speed;

    if (enemy->position.x < area->active_zone->x + enemy->radius) {
        enemy->position.x = area->active_zone->x + enemy->radius;
        oscillating_set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.x > area->active_zone->x + area->active_zone->width - enemy->radius) {
        enemy->position.x = area->active_zone->x + area->active_zone->width - enemy->radius;
        oscillating_set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.y < area->active_zone->y + enemy->radius) {
        enemy->position.y = area->active_zone->y + enemy->radius;
        oscillating_set_angle(movement_data, 2*PI - movement_data->angle);
    }
    if (enemy->position.y > area->active_zone->y + area->active_zone->height - enemy->radius) {
        enemy->position.y = area->active_zone->y + area->active_zone->height - enemy->radius;
        oscillating_set_angle(movement_data, 2*PI - movement_data->angle);
    }
}