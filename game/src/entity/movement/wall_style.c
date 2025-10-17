#include "movement.h"
#include <math.h>
#include <malloc.h>

typedef struct WallMovementData {
    float vx;
    float vy;
    int8_t movement_direction;
} WallMovementData;

enum WallMovementDirection {
    MOVEMENT_CLOCKWISE = 1,
    MOVEMENT_COUNTERCLOCKWISE = -1,
};

void set_wall_movement_data(Area* area, Enemy* enemy, int8_t movement_direction) {
    WallMovementData* movement_data = malloc(sizeof(WallMovementData));
    movement_data->movement_direction = movement_direction;
    if (enemy->position.x == area->active_zone->x + enemy->radius) {
        movement_data->vx = 0;
        movement_data->vy = -movement_data->movement_direction;
    } else if (enemy->position.y == area->active_zone->y + enemy->radius) {
        movement_data->vx = movement_data->movement_direction;
        movement_data->vy = 0;
    } else if (enemy->position.x == area->active_zone->x + area->active_zone->width - enemy->radius) {
        movement_data->vx = 0;
        movement_data->vy = movement_data->movement_direction;
    } else {
        movement_data->vx = -movement_data->movement_direction;
        movement_data->vy = 0;
    }
    enemy->movement_data = movement_data;
}

void wall_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        set_wall_movement_data(area, enemy, MOVEMENT_CLOCKWISE);
    }

    WallMovementData* movement_data = (WallMovementData*) enemy->movement_data;
    const float frame_speed = enemy->base_speed * GetFrameTime();
    enemy->position.x += movement_data->vx * frame_speed;
    enemy->position.y += movement_data->vy * frame_speed;

    if (enemy->position.x < area->active_zone->x + enemy->radius) {
        enemy->position.x = area->active_zone->x + enemy->radius;
        movement_data->vx = 0;
        movement_data->vy = -movement_data->movement_direction;
    }
    if (enemy->position.x > area->active_zone->x + area->active_zone->width - enemy->radius) {
        enemy->position.x = area->active_zone->x + area->active_zone->width - enemy->radius;
        movement_data->vx = 0;
        movement_data->vy = movement_data->movement_direction;
    }
    if (enemy->position.y < area->active_zone->y + enemy->radius) {
        enemy->position.y = area->active_zone->y + enemy->radius;
        movement_data->vx = movement_data->movement_direction;
        movement_data->vy = 0;
    }
    if (enemy->position.y > area->active_zone->y + area->active_zone->height - enemy->radius) {
        enemy->position.y = area->active_zone->y + area->active_zone->height - enemy->radius;
        movement_data->vx = -movement_data->movement_direction;
        movement_data->vy = 0;
    }
}