#include "state.h"
#include <stdio.h>
#include <assert.h>

void open_map(GameState* state) {
    FILE* file = fopen("maps/world.bin", "rb");
    assert(file != NULL);
    load_map(&state->map, file);
    fclose(file);
    printf("Loaded regions: %d\nLoaded spawn area is at: %d, %d\n", state->map.region_count, state->map.spawn_area->x, state->map.spawn_area->y);
}