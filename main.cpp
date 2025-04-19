#include <iostream>

#include "raylib.h"
#include "raymath.h"

int getState(int a, int b, int c, int d) {
    return a * 8 + b * 4 + c * 2 + d * 1;
}

void drawIsoLines(Vector2 v1, Vector2 v2) {
    DrawLineEx(v1, v2, 2, WHITE);
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

    float circleRadius = 100.0;
    bool showCircle = false;

    // Global loop.
    while (!WindowShouldClose()) {

        float scrollValue = GetMouseWheelMove();
        circleRadius += scrollValue * 10.0f;

        if (IsKeyPressed(KEY_R)) {
            showCircle = !showCircle;
        }

        if (showCircle) {
            DrawCircleLinesV(Vector2(GetMouseX(), GetMouseY()), circleRadius, BLUE);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawCircleLinesV(Vector2(GetMouseX(), GetMouseY()), circleRadius, BLUE);
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    if (Vector2Distance(Vector2(i * resolution, j * resolution), Vector2(GetMouseX(), GetMouseY())) < circleRadius) {
                        plane[i][j] = 0;
                    }
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            DrawCircleLinesV(Vector2(GetMouseX(), GetMouseY()), circleRadius, BLUE);
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    if (Vector2Distance(Vector2(i * resolution, j * resolution), Vector2(GetMouseX(), GetMouseY())) < circleRadius) {
                        plane[i][j] = 1;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw corners.
        // for (int i = 0; i < cols; ++i) {
        //     for (int j = 0; j < rows; ++j) {
        //         // Color corner based on value.
        //         if (plane[i][j] == 0) {
        //             DrawCircle(i * resolution, j * resolution, 2, BLACK);
        //         } else {
        //             DrawCircle(i * resolution, j * resolution, 2, WHITE);
        //         }
        //         // DrawCircle(i * resolution, j * resolution, 3, Color(255, 255, 255, plane[i][j]*255));
        //     }
        // }

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

                switch (state) {
                    case 1:
                        drawIsoLines(c, d);
                        break;
                    case 2:
                        drawIsoLines(b, c);
                        break;
                    case 3:
                        drawIsoLines(b, d);
                        break;
                    case 4:
                        drawIsoLines(a, b);
                        break;
                    case 5:
                        drawIsoLines(a, d);
                        drawIsoLines(b, c);
                        break;
                    case 6:
                        drawIsoLines(a, c);
                        break;
                    case 7:
                        drawIsoLines(a, d);
                        break;
                    case 8:
                        drawIsoLines(a, d);
                        break;
                    case 9:
                        drawIsoLines(a, c);
                        break;
                    case 10:
                        drawIsoLines(a, b);
                        drawIsoLines(c, d);
                        break;
                    case 11:
                        drawIsoLines(a, b);
                        break;
                    case 12:
                        drawIsoLines(b, d);
                        break;
                    case 13:
                        drawIsoLines(b, c);
                        break;
                    case 14:
                        drawIsoLines(c, d);
                        break;
                    default: break;
                }
            }
        }

        EndDrawing();
    }

    return 0;
}
