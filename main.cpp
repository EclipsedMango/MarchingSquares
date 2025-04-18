#include <iostream>

#include "raylib.h"

int getState(int a, int b, int c, int d) {
    return a * 8 + b * 4 + c * 2 + d * 1;
}

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

    int resolution = 20;

    // Add 1 to extend to edges of window.
    int cols = 1 + windowWidth / resolution;
    int rows = 1 + windowHeight / resolution;

    int plane[cols][rows];

    // Create corners on a plane/grid.
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            plane[i][j] = GetRandomValue(0, 1);
        }
    }

    // Global Game loop.
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(GRAY);

        // Draw corners.
        for (int i = 0; i < cols; ++i) {
            for (int j = 0; j < rows; ++j) {
                // Color corner based on value.
                if (plane[i][j] == 0) {
                    DrawCircle(i * resolution, j * resolution, 3, BLACK);
                } else {
                    DrawCircle(i * resolution, j * resolution, 3, WHITE);
                }
                // DrawCircle(i * resolution, j * resolution, 3, Color(255, 255, 255, plane[i][j]*255));
            }
        }

        // Draw Lines. - 1 to Account for + 1 at initialisation.
        for (int i = 0; i < cols - 1; ++i) {
            for (int j = 0; j < rows - 1; ++j) {
                int x = i * resolution;
                int y = j * resolution;

                Vector2 a = Vector2(x + resolution / 2.0, y);
                Vector2 b = Vector2(x + resolution, y + resolution / 2.0);
                Vector2 c = Vector2(x + resolution / 2.0, y + resolution);
                Vector2 d = Vector2(x, y + resolution / 2.0);

                int state = getState(plane[i][j], plane[i + 1][j], plane[i + 1][j + 1], plane[i][j + 1]);
            }
        }

        EndDrawing();
    }

    return 0;
}
