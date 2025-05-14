#include "movement.h"
#include "../util/random.h"
#include <math.h>
#include <malloc.h>

typedef struct NormalMovementData {
    float angle;
    float vx;
    float vy;
} NormalMovementData;

void normal_movement(Area* area, Enemy* enemy) {
    if (enemy->movement_data == NULL) {
        NormalMovementData* movement_data = malloc(sizeof(NormalMovementData));
        movement_data->angle = uniform_random(0, 2*PI);
        movement_data->vx = cosf(movement_data->angle);
        movement_data->vy = sinf(movement_data->angle);
        enemy->movement_data = movement_data;
    }

    const NormalMovementData* movement_data = (NormalMovementData*) enemy->movement_data;
    const float frame_speed = enemy->base_speed * GetFrameTime();
    enemy->position.x += movement_data->vx * frame_speed;
    enemy->position.y += movement_data->vy * frame_speed;
}