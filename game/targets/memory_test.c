#include "../src/state.h"
#define TEST_COUNT 1

int main(void) {
    GameState state;
    for (int i = 0; i < TEST_COUNT; i++) {
        open_map(&state);
        destroy_map(&state.map);
    }
    printf("Map created and destroyed.\n");
    return 0;
}