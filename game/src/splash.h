#pragma once

#include <stdint.h>

#define MAX_SPLASH_MESSAGES 8
#define MAX_SPLASH_LENGTH 64

// Splash messages data structure:

typedef struct SplashMessage {
    char text[MAX_SPLASH_LENGTH];
    float duration;
} SplashMessage;

typedef struct SplashMessages {
    SplashMessage messages[MAX_SPLASH_MESSAGES];
    uint8_t stored_messages;
    uint8_t start;
} SplashMessages;

void init_splash_messages(SplashMessages* splash_messages);
void add_splash_message(SplashMessages* splash_messages, const char* message);
void process_splash_messages(SplashMessages* splash_messages);