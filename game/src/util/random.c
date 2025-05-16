#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// Implementation: https://lomont.org/papers/2008/Lomont_PRNG_2008.pdf
static unsigned long state[16];
static unsigned int index = 0;
unsigned long WELLRNG512(void) {
    unsigned long a, b, c, d;
    a = state[index];
    c = state[(index+13)&15];
    b = a^c^(a<<16)^(c<<15);
    c = state[(index+9)&15];
    c ^= (c>>11);
    a = state[index] = b^c;
    d = a^((a<<5)&0xDA442D24UL);
    index = (index + 15)&15;
    a = state[index];
    state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
    return state[index];
}

void init_random() {
    index = 0;
    FILE* urandom = fopen("/dev/urandom", "rb");
    fread(state, sizeof(unsigned long), 16, urandom);
    fclose(urandom);
}

float uniform_random(float min, float max) {
    return ((double)WELLRNG512() / UINT64_MAX) * (max - min) + min;
}

int discrete_random(int min, int max) {
    return floorf(uniform_random(min, max));
}