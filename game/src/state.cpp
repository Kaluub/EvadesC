#include "state.hpp"
#include <stdio.h>
#include <assert.h>

void GameState::read_map() {
    FILE* file = fopen("maps/world.bin", "rb");
    assert(file != NULL);
    map.load(file);
    fclose(file);
    printf("Loaded regions: %d\nLoaded spawn area is at: %d, %d\n", map.region_count, map.spawn_area->x, map.spawn_area->y);
}