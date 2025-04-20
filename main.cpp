#include <iostream>
#include <external/stb_perlin.h>

#include "raylib.h"
#include "raymath.h"

int getState(int a, int b, int c, int d) {
    // Bit shifting.
    // return d | c << 1 | b << 2 | a << 3;
    return a * 8 + b * 4 + c * 2 + d * 1;
}

void drawIsoLines(Vector2 v1, Vector2 v2) {
    DrawLineEx(v1, v2, 2, WHITE);
}

int main() {
    // These should only be initialized here, so we can have menus.
    const int monitor = GetCurrentMonitor();
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    int windowWidth = 1920;
    int windowHeight = 1080;

    InitWindow(windowWidth, windowHeight, "MarchingSquares");

    SetWindowMonitor(monitor);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    int resolution = 10;

    // Add 1 to extend to edges of window.
    int cols = 1 + windowWidth / resolution;
    int rows = 1 + windowHeight / resolution;

    float plane[cols][rows];

    float increment = 0.1;
    float offsetZ = 0;

    float circleRadius = 100.0;

    bool showCircle = false;
    bool showGridCorners = false;
    bool moveNoise = false;

    // Generate noise once at startup.
    float firstOffsetX = 0;
    for (int i = 0; i < cols; ++i) {
        firstOffsetX += increment;
        float firstOffsetY = 0;
        for (int j = 0; j < rows; ++j) {
            firstOffsetY += increment;
            plane[i][j] = stb_perlin_fbm_noise3(firstOffsetX, firstOffsetY, 0.0, 1.0, 0.5, 2);
        }
    }

    // Global loop.
    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();

        float scrollValue = GetMouseWheelMove();
        circleRadius += scrollValue * 10.0f;

        // Controls.
        if (IsKeyPressed(KEY_R)) {
            showCircle = !showCircle;
        } else if (IsKeyPressed(KEY_E)) {
            showGridCorners = !showGridCorners;
        } else if (IsKeyPressed(KEY_T)) {
            moveNoise = !moveNoise;
        }

        if (showCircle) {
            DrawCircleLinesV(Vector2(GetMouseX(), GetMouseY()), circleRadius, BLUE);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            DrawCircleLinesV(Vector2(GetMouseX(), GetMouseY()), circleRadius, BLUE);
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    if (Vector2Distance(Vector2(i * resolution, j * resolution), Vector2(GetMouseX(), GetMouseY())) < circleRadius) {
                        plane[i][j] = 0;
                    }
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawCircleLinesV(Vector2(GetMouseX(), GetMouseY()), circleRadius, BLUE);
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    if (Vector2Distance(Vector2(i * resolution, j * resolution), Vector2(GetMouseX(), GetMouseY())) < circleRadius) {
                        plane[i][j] = 1;
                    }
                }
            }
        }

        // Drawing.
        BeginDrawing();
        ClearBackground(BLACK);

        // Enable 3D Noise.
        if (moveNoise) {
            float offsetX = 0;
            for (int i = 0; i < cols; ++i) {
                offsetX += increment;
                float offsetY = 0;
                for (int j = 0; j < rows; ++j) {
                    plane[i][j] = stb_perlin_fbm_noise3(offsetX, offsetY, offsetZ, 1.0, 0.5, 2);
                    offsetY += increment;
                }
            }
            offsetZ += 0.5 * delta;
        }

        // Draw corners.
        if (showGridCorners) {
            for (int i = 0; i < cols; ++i) {
                for (int j = 0; j < rows; ++j) {
                    // Color corner based on value.
                    DrawRectangle(i * resolution, j * resolution,
                        resolution,
                        resolution,
                        Color(255, 255, 255, plane[i][j] * 255)
                    );
                }
            }
        }

        // Draw Lines. - 1 to Account for + 1 at initialisation.
        for (int i = 0; i < cols - 1; ++i) {
            for (int j = 0; j < rows - 1; ++j) {
                int x = i * resolution;
                int y = j * resolution;

                int state = getState(
                    ceil(plane[i][j]),
                    ceil(plane[i + 1][j]),
                    ceil(plane[i + 1][j + 1]),
                    ceil(plane[i][j + 1])
                );

                float aVal = plane[i][j] + 1;
                float bVal = plane[i + 1][j] + 1;
                float cVal = plane[i + 1][j + 1] + 1;
                float dVal = plane[i][j + 1] + 1;

                Vector2 a = Vector2(lerp(x, x + resolution, (1 - aVal) / (bVal - aVal)), y);
                Vector2 b = Vector2(x + resolution, lerp(y, y + resolution, (1 - bVal) / (cVal - bVal)));
                Vector2 c = Vector2(lerp(x, x + resolution, (1 - dVal) / (cVal - dVal)), y + resolution);
                Vector2 d = Vector2(x, lerp(y, y + resolution, (1 - aVal) / (dVal - aVal)));

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

        DrawFPS(25, 25);
        EndDrawing();
    }

    return 0;
}
