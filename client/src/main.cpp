#include "raylib.h"

int main() {
    // Initialize the window
    InitWindow(800, 600, "Raylib Window Example");

    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        // Start drawing
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw some basic text
        DrawText("Hello, Raylib!", 200, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}
