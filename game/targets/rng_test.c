#include "../src/util/random.h"
#include <stdlib.h>
#include <stdio.h>

#define TEST_COUNT 100
#define DISTRIBUTION_COUNT 5
#define DISTRIBUTION_ITERATIONS 1000000*DISTRIBUTION_COUNT

int main() {
    init_random();
    printf("Random unit test: calling discrete_random(0, 1) -> always 0 expected\n");
    for (int i = 0; i < TEST_COUNT; i++) {
        printf("%d ", discrete_random(0, 1));
    }
    printf("\nRandom unit test: calling discrete_random(0, 2) -> 0 or 1 expected\n");
    for (int i = 0; i < TEST_COUNT; i++) {
        printf("%d ", discrete_random(0, 2));
    }
    printf("\nRandom unit test: calling uniform_random(-1, 1) -> [-1, 1) value expected\n");
    double sum = 0;
    for (int i = 0; i < TEST_COUNT; i++) {
        double value = uniform_random(-1, 1);
        printf("%f ", value);
        sum += value;
    }
    printf("\n - sum from above: %f (expected: follows a normal distribution about 0)\n", sum);

    // Distribution test
    printf("Distribution test: Disregard results if above fails\n");
    int results[DISTRIBUTION_COUNT] = {0};
    for (int i = 0; i < DISTRIBUTION_ITERATIONS; i++) {
        results[discrete_random(0, DISTRIBUTION_COUNT)]++;
    }
    for (int i = 0; i < DISTRIBUTION_COUNT; i++) {
        printf(" - %d: %d\n", i, results[i]);
    }

    // Random chance test
    int successes = 0;
    int rolls = 0;
    while (rolls < 10000000) {
        rolls++;
        if (uniform_random(0, 1) < 0.005) {
            successes++;
        }
    }
    printf("Random chance test: %d successes/%d rolls (%.5f)\n", successes, rolls, (double)successes/rolls);
    return EXIT_SUCCESS;
}