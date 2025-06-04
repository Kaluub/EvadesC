#include "behaviour.h"
#include "../movement/movement.h"
#include <malloc.h>
#include <math.h>

typedef struct RadiatingBulletsBehaviourData {
    float release_time;
} RadiatingBulletsBehaviourData;

Color projectile_color = {163, 8, 56, 255};

void radiating_bullets_behaviour(Area* area, Enemy* enemy) {
    if (enemy->behaviour_data == NULL) {
        RadiatingBulletsBehaviourData* behaviour_data = (RadiatingBulletsBehaviourData*) malloc(sizeof(RadiatingBulletsBehaviourData));
        behaviour_data->release_time = uniform_random(0, 3.5);
        enemy->behaviour_data = behaviour_data;
    }

    RadiatingBulletsBehaviourData* behaviour_data = enemy->behaviour_data;
    behaviour_data->release_time += GetFrameTime();
    if (behaviour_data->release_time > 3.5) {
        behaviour_data->release_time -= 3.5;
        for (int i = 1; i <= 8; i++) {
            Enemy* projectile = enemy_manual_init(normal_movement, projectile_color);
            projectile->base_speed = 240;
            projectile->radius = 8;
            projectile->duration = 70.0f/24;
            projectile->wall_behaviour = WALL_BEHAVIOUR_REMOVE;
            NormalMovementData* projectile_movement_data = (NormalMovementData*) malloc(sizeof(NormalMovementData));
            float angle = PI * i / 4;
            set_angle(projectile_movement_data, angle);
            projectile->position.x = enemy->position.x + enemy->radius * projectile_movement_data->vx;
            projectile->position.y = enemy->position.y + enemy->radius * projectile_movement_data->vy;
            projectile->movement_data = projectile_movement_data;

            enemy_set_add(area->enemy_set, projectile);
        }
    }
}