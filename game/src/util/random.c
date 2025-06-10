#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#ifdef WINDOWS
#include <time.h>
#endif

// Implementation: https://lomont.org/papers/2008/Lomont_PRNG_2008.pdf
static uint64_t state[16];
static uint32_t index = 0;
uint64_t WELLRNG512(void) {
    uint64_t a, b, c, d;
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
#ifndef WINDOWS
    FILE* urandom = fopen("/dev/urandom", "rb");
    fread(state, sizeof(uint64_t), 16, urandom);
    fclose(urandom);
#else
    // Windows hack because I don't really care about it much
    // Plus in the future a server should be controlling all RNG
    for (int i = 0; i < 16; i++) {
        state[i] = time(NULL) + 13*i;
    }
#endif
}

double uniform_random(double min, double max) {
    return (double)(WELLRNG512() & 0x001FFFFFFFFFFFFF) / (1ull<<53ull) * (max - min) + min;
}

int32_t discrete_random(int32_t min, int32_t max) {
    return WELLRNG512() % (max - min) + min;
}