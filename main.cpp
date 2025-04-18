#include <iostream>

#include "raylib.h"

int main() {
    // These should only be initialized here, so we can have menus.
    const int monitor = GetCurrentMonitor();
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    constexpr float physicsDelta = 1.0f / 240.0f;
    float physicsTimer = 0.0f;

    int windowWidth = 1920;
    int windowHeight = 1080;

    InitWindow(windowWidth, windowHeight, "Rays");

    SetWindowMonitor(monitor);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Global Game loop.
    while (!WindowShouldClose()) {

    }

    return 0;
}
