#include "movement.h"
#include <math.h>
#include <malloc.h>

typedef struct NormalMovementData {
    float angle;
    float vx;
    float vy;
} NormalMovementData;

void set_angle(NormalMovementData* data, float angle) {
    if (angle > 2*PI) {
        angle -= 2*PI;
    }
    data->angle = angle;
    data->vx = cosf(angle);
    data->vy = sinf(angle);
}

void normal_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        NormalMovementData* movement_data = malloc(sizeof(NormalMovementData));
        set_angle(movement_data, uniform_random(0, 2*PI));
        enemy->movement_data = movement_data;
    }

    NormalMovementData* movement_data = (NormalMovementData*) enemy->movement_data;
    const float frame_speed = enemy->base_speed * GetFrameTime();
    enemy->position.x += movement_data->vx * frame_speed;
    enemy->position.y += movement_data->vy * frame_speed;

    if (enemy->position.x < area->active_zone->x + enemy->radius) {
        enemy->position.x = area->active_zone->x + enemy->radius;
        set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.x > area->active_zone->x + area->active_zone->width - enemy->radius) {
        enemy->position.x = area->active_zone->x + area->active_zone->width - enemy->radius;
        set_angle(movement_data, PI - movement_data->angle);
    }
    if (enemy->position.y < area->active_zone->y + enemy->radius) {
        enemy->position.y = area->active_zone->y + enemy->radius;
        set_angle(movement_data, 2*PI - movement_data->angle);
    }
    if (enemy->position.y > area->active_zone->y + area->active_zone->height - enemy->radius) {
        enemy->position.y = area->active_zone->y + area->active_zone->height - enemy->radius;
        set_angle(movement_data, 2*PI - movement_data->angle);
    }
}