#include "raylib.h"

int main() {
    // Initialize window
    InitWindow(800, 600, "Blue Square Example");

    // Set target FPS
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update (nothing needed here for now)

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw a blue square (x, y, width, height, color)
        DrawRectangle(350, 250, 100, 100, BLUE);

        EndDrawing();
    }

    // Close window
    CloseWindow();

    return 0;
}