#pragma once

#include <stdint.h>

void init_random();
double uniform_random(double min, double max);
int32_t discrete_random(int32_t min, int32_t max);