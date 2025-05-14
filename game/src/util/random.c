#include <stdlib.h>

float uniform_random(float min, float max) {
    return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}