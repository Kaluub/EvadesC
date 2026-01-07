#include "splash.hpp"
#include "raylib.h"
#include <string.h>

SplashMessages::SplashMessages() {
    stored_messages = 0;
    start = 0;
}

void SplashMessages::add(const char* message) {
    SplashMessage* element = messages + (start + stored_messages) % MAX_SPLASH_MESSAGES;
    element->duration = 2500;
    element->text[MAX_SPLASH_LENGTH - 1] = 0;
    strncpy(element->text, message, MAX_SPLASH_LENGTH - 2);
    if (stored_messages < MAX_SPLASH_MESSAGES) {
        stored_messages++;
    } else {
        start = (start + 1) % MAX_SPLASH_MESSAGES;
    }
}

void SplashMessages::process() {
    // Remove expired messages
    while (stored_messages > 0 && messages[start].duration <= 0) {
        start = (start + 1) % MAX_SPLASH_MESSAGES;
        stored_messages--;
    }
    // Update and draw splash messages
    int y = 30;
    for (int i = stored_messages - 1; i >= 0; i--) {
        int index = (start + i) % MAX_SPLASH_MESSAGES;
        SplashMessage* message = messages + index;
        message->duration -= GetFrameTime() * 1000;
        if (message->duration <= 0) {
            continue;
        }
        uint8_t alpha = (message->duration > 500) ? 255 : (message->duration * 0.51);
        uint8_t value = 220 + (message->duration > 2000 ? (message->duration - 2000)*0.06 : 0);
        Color text_color = {value, value, value, alpha};
        Color shadow_color = {0, 0, 0, alpha};
        DrawText(message->text, 12, y + 2, 20, shadow_color);
        DrawText(message->text, 10, y, 20, text_color);
        y += 24;
    }
}