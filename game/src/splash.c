#include "splash.h"
#include "raylib.h"
#include <string.h>

void init_splash_messages(SplashMessages* splash_messages) {
    splash_messages->stored_messages = 0;
    splash_messages->start = 0;
}

void add_splash_message(SplashMessages* splash_messages, const char* message) {
    SplashMessage* element = splash_messages->messages + (splash_messages->start + splash_messages->stored_messages) % MAX_SPLASH_MESSAGES;
    element->duration = 2500;
    element->text[MAX_SPLASH_LENGTH - 1] = 0;
    strncpy(element->text, message, MAX_SPLASH_LENGTH - 2);
    if (splash_messages->stored_messages < MAX_SPLASH_MESSAGES) {
        splash_messages->stored_messages++;
    } else {
        splash_messages->start = (splash_messages->start + 1) % MAX_SPLASH_MESSAGES;
    }
}

void process_splash_messages(SplashMessages* splash_messages) {
    // Remove expired messages
    while (splash_messages->stored_messages > 0 && splash_messages->messages[splash_messages->start].duration <= 0) {
        splash_messages->start = (splash_messages->start + 1) % MAX_SPLASH_MESSAGES;
        splash_messages->stored_messages--;
    }
    // Update and draw splash messages
    int y = 30;
    for (int i = splash_messages->stored_messages - 1; i >= 0; i--) {
        int index = (splash_messages->start + i) % MAX_SPLASH_MESSAGES;
        SplashMessage* message = splash_messages->messages + index;
        message->duration -= GetFrameTime() * 1000;
        if (message->duration <= 0) {
            continue;
        }
        uint8_t alpha = (message->duration > 500) ? 255 : (message->duration * 0.51);
        uint8_t value = 200 + (message->duration > 2000 ? (message->duration - 2000)*0.11 : 0);
        Color text_color = {value, value, value, alpha};
        Color shadow_color = {0, 0, 0, alpha};
        DrawText(message->text, 12, y + 2, 20, shadow_color);
        DrawText(message->text, 10, y, 20, text_color);
        y += 24;
    }
}