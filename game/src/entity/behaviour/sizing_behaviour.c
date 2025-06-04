#include "behaviour.h"
#include <malloc.h>

typedef struct SizingBehaviourData {
    float base_radius;
    float sizing_time;
} SizingBehaviourData;


void sizing_behaviour(Area* area, Enemy* enemy) {
    if (enemy->behaviour_data == NULL) {
        SizingBehaviourData* behaviour_data = (SizingBehaviourData*) malloc(sizeof(SizingBehaviourData));
        behaviour_data->base_radius = enemy->radius;
        behaviour_data->sizing_time = 0;
        enemy->behaviour_data = behaviour_data;
    }

    SizingBehaviourData* behaviour_data = (SizingBehaviourData*) enemy->behaviour_data;
    behaviour_data->sizing_time += GetFrameTime();
    if (behaviour_data->sizing_time > 3.5) {
        behaviour_data->sizing_time -= 3.5;
    }
    float radius_multiplier = behaviour_data->sizing_time < 1.75 ? 0.4 + 1.2 * behaviour_data->sizing_time : 2.5 - 1.2 * (behaviour_data->sizing_time - 1.75);
    enemy->radius = radius_multiplier * behaviour_data->base_radius;
}