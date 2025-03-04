#include "state.h"
#include <stdio.h>

void open_map(GameState* state) {
    FILE* file = fopen("maps/world.bin", "rb");
    load_map(&state->map, file);
    fclose(file);
    printf("Loaded spawn region name: %s\nLoaded regions: %d\n", state->map.spawn_region, state->map.region_count);
}