#include "movement.h"
#include "../util/random.h"
#include <math.h>
#include <malloc.h>

typedef struct WallMovementData {
    float vx;
    float vy;
} WallMovementData;

void wall_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        WallMovementData* movement_data = malloc(sizeof(WallMovementData));
        if (enemy->position.x == area->active_zone->x + enemy->radius) {
            movement_data->vx = 0;
            movement_data->vy = -1;
        } else if (enemy->position.y == area->active_zone->y + enemy->radius) {
            movement_data->vx = 1;
            movement_data->vy = 0;
        } else if (enemy->position.x == area->active_zone->x + area->active_zone->width - enemy->radius) {
            movement_data->vx = 0;
            movement_data->vy = 1;
        } else {
            movement_data->vx = -1;
            movement_data->vy = 0;
        }
        enemy->movement_data = movement_data;
    }

    WallMovementData* movement_data = (WallMovementData*) enemy->movement_data;
    const float frame_speed = enemy->base_speed * GetFrameTime();
    enemy->position.x += movement_data->vx * frame_speed;
    enemy->position.y += movement_data->vy * frame_speed;

    if (enemy->position.x < area->active_zone->x + enemy->radius) {
        enemy->position.x = area->active_zone->x + enemy->radius;
        movement_data->vx = 0;
        movement_data->vy = -1;
    }
    if (enemy->position.x > area->active_zone->x + area->active_zone->width - enemy->radius) {
        enemy->position.x = area->active_zone->x + area->active_zone->width - enemy->radius;
        movement_data->vx = 0;
        movement_data->vy = 1;
    }
    if (enemy->position.y < area->active_zone->y + enemy->radius) {
        enemy->position.y = area->active_zone->y + enemy->radius;
        movement_data->vx = 1;
        movement_data->vy = 0;
    }
    if (enemy->position.y > area->active_zone->y + area->active_zone->height - enemy->radius) {
        enemy->position.y = area->active_zone->y + area->active_zone->height - enemy->radius;
        movement_data->vx = -1;
        movement_data->vy = 0;
    }
}