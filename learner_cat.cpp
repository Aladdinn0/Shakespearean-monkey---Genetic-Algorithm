#include <raylib.h>

int main() {
    char letters[26] = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','z'
    };

    InitWindow(800, 450, "Learning to Find CAT");

    SetTargetFPS(30);
    SetRandomSeed(432);

    // Current letters
    char firstLetter = ' ';
    char secondLetter = ' ';
    char thirdLetter = ' ';

    // Lock states (this is the "learning")
    bool foundC = false;
    bool foundA = false;
    bool foundT = false;

    int frameCounter = 0;

    double startTime = GetTime();
    double finalTime = 0;

    while (!WindowShouldClose()) {

        double currentTime = GetTime() - startTime;

        // --- LOGIC ---
        if (!(foundC && foundA && foundT)) {
            frameCounter++;

            if (frameCounter >= 2) {

                // Only randomize letters that are NOT found yet
                if (!foundC) {
                    firstLetter = letters[GetRandomValue(0, 25)];
                    if (firstLetter == 'c') {
                        foundC = true;
                    }
                }

                if (!foundA) {
                    secondLetter = letters[GetRandomValue(0, 25)];
                    if (secondLetter == 'a') {
                        foundA = true;
                    }
                }

                if (!foundT) {
                    thirdLetter = letters[GetRandomValue(0, 25)];
                    if (thirdLetter == 't') {
                        foundT = true;
                    }
                }

                // If all found, store final time
                if (foundC && foundA && foundT) {
                    finalTime = currentTime;
                }

                frameCounter = 0;
            }
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Timer (live or final)
        if (foundC && foundA && foundT) {
            DrawText(TextFormat("Time: %.2f", finalTime), 100, 100, 30, GREEN);
            DrawText("Found it!", 300, 150, 30, DARKGRAY);
        } else {
            DrawText(TextFormat("Time: %.2f", currentTime), 100, 100, 30, DARKGRAY);
            DrawText("Learning...", 300, 150, 30, DARKGRAY);
        }

        // Draw letters (green if locked, red if still searching)
        DrawText(TextFormat("%c", firstLetter), 320, 200, 60, foundC ? GREEN : RED);
        DrawText(TextFormat("%c", secondLetter), 360, 200, 60, foundA ? GREEN : RED);
        DrawText(TextFormat("%c", thirdLetter), 400, 200, 60, foundT ? GREEN : RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}