#include "../src/state.h"
#include "../src/util/random.h"
#include <stdlib.h>

#define TEST_COUNT 1
#define FRAME_SIMULATIONS 100

int main(void) {
    init_random();
    GameState state;
    for (int i = 0; i < TEST_COUNT; i++) {
        open_map(&state);
        for (int j = 0; j < FRAME_SIMULATIONS; j++) {
            for (int region_index = 0; region_index < state.map.region_count; region_index++) {
                Region* region = state.map.regions + region_index;
                for (int area_index = 0; area_index < region->area_count; area_index++) {
                    Area* area = region->areas + area_index;
                    enemy_set_update(area->enemy_set, area);
                }
            }
        }
        destroy_map(&state.map);
    }
    printf("Map created and destroyed.\n");
    return EXIT_SUCCESS;
}