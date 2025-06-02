#include "movement.h"
#include <math.h>
#include <malloc.h>

typedef struct IcicleMovementData {
    float vy;
    float wall_time;
} IcicleMovementData;

void icicle_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        IcicleMovementData* movement_data = malloc(sizeof(IcicleMovementData));
        movement_data->vy = uniform_random(0, 1) < 0.5 ? -1 : 1;
        movement_data->wall_time = 0;
        enemy->movement_data = movement_data;
    }

    IcicleMovementData* movement_data = (IcicleMovementData*) enemy->movement_data;

    if (movement_data->wall_time > 0) {
        movement_data->wall_time -= 1000 * GetFrameTime();
        return;
    }

    const float frame_speed = enemy->base_speed * GetFrameTime();
    enemy->position.y += movement_data->vy * frame_speed;

    if (enemy->position.x < area->active_zone->x + enemy->radius) {
        enemy->position.x = area->active_zone->x + enemy->radius;
    }
    if (enemy->position.x > area->active_zone->x + area->active_zone->width - enemy->radius) {
        enemy->position.x = area->active_zone->x + area->active_zone->width - enemy->radius;
    }
    if (enemy->position.y < area->active_zone->y + enemy->radius) {
        enemy->position.y = area->active_zone->y + enemy->radius;
        movement_data->wall_time = 1000;
        movement_data->vy *= -1;
    }
    if (enemy->position.y > area->active_zone->y + area->active_zone->height - enemy->radius) {
        enemy->position.y = area->active_zone->y + area->active_zone->height - enemy->radius;
        movement_data->wall_time = 1000;
        movement_data->vy *= -1;
    }
}