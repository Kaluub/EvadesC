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
    for (int i = 0; i < splash_messages->stored_messages; i++) {
        int index = (splash_messages->start + i) % MAX_SPLASH_MESSAGES;
        splash_messages->messages[index].duration -= GetFrameTime() * 1000;
        if (splash_messages->messages[index].duration <= 0) {
            continue;
        }
        float alpha = (splash_messages->messages[index].duration > 500) ? 1 : (splash_messages->messages[index].duration / 500);
        DrawText(splash_messages->messages[index].text, 12, y + 2, 20, ColorAlpha(BLACK, alpha));
        DrawText(splash_messages->messages[index].text, 10, y, 20, ColorAlpha(WHITE, alpha));
        y += 24;
    }
}