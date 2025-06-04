#include "behaviour.h"
#include <malloc.h>

typedef struct SwitchBehaviourData {
    float switch_interval;
    float switch_time;
} SwitchBehaviourData;

void switch_behaviour(Area* area, Enemy* enemy) {
    if (enemy->behaviour_data == NULL) {
        SwitchBehaviourData* behaviour_data = (SwitchBehaviourData*) malloc(sizeof(SwitchBehaviourData));
        behaviour_data->switch_interval = 3;
        behaviour_data->switch_time = uniform_random(0, 1) < 0.5 ? 0 : behaviour_data->switch_interval;
        enemy->behaviour_data = behaviour_data;
    }

    SwitchBehaviourData* behaviour_data = (SwitchBehaviourData*) enemy->behaviour_data;
    behaviour_data->switch_time += GetFrameTime();
    if (behaviour_data->switch_time > 2*behaviour_data->switch_interval) {
        behaviour_data->switch_time -= 2*behaviour_data->switch_interval;
    }
    if (behaviour_data->switch_time > behaviour_data->switch_interval) {
        enemy->harmless = true;
    }
}