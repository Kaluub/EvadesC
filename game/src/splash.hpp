#pragma once

#include <stdint.h>

#define MAX_SPLASH_MESSAGES 8
#define MAX_SPLASH_LENGTH 128

// Splash messages data structure:

struct SplashMessage {
    char text[MAX_SPLASH_LENGTH];
    float duration;
};

struct SplashMessages {
    SplashMessage messages[MAX_SPLASH_MESSAGES];
    uint8_t stored_messages;
    uint8_t start;

    SplashMessages();
    void add(const char* message);
    void process();
};