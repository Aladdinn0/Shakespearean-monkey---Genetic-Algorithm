#include <raylib.h>

int main() {
    char letters[26] = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','z'
    };

    InitWindow(800, 450, "Find the Cat");
    
    // Limits the game to 60 frames per second so our math is predictable
    SetTargetFPS(6000); 
    SetRandomSeed(4221);

    // --- NEW STATE VARIABLES ---
    char current1Letter = ' ';  // Holds the letter we are currently testing
    char current2Letter = ' ';  // Holds the letter we are currently testing
    char current3Letter = ' ';  // Holds the letter we are currently testing
    
    bool found = false;       // Switch to tell us when to stop
    int frameCounter = 0;      // Acts as our timer
    double startTime = GetTime();
    double elapsedTime = 0;


    while (!WindowShouldClose()) {
        // --- LOGIC PHASE ---
        double currentTime = GetTime() - startTime;

        // Only keep picking new letters if we HAVEN'T found 'cat' yet
        if (!found) {
            frameCounter++;
            
            // Wait 10 frames (about 1/6th of a second) before trying a new letter.
            // This slows it down enough for your eyes to see the "writing and deleting".
            if (frameCounter >= 2) {
                current1Letter = letters[GetRandomValue(0, 25)];
                current2Letter = letters[GetRandomValue(0, 25)];
                current3Letter = letters[GetRandomValue(0, 25)];

                // Check if we hit the jackpot
                if ((current1Letter == 'c') && (current2Letter == 'a') &&  (current3Letter == 't')) {
                    found = true; 
                    elapsedTime = GetTime() - startTime;
                }               

                // Reset the timer for the next guess
                frameCounter = 0; 
            }
        }

        // --- DRAWING PHASE ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // If we found it, draw it in green and stop!
        if (found) {
            DrawText("Found it!", 300, 150, 30, DARKGRAY);
            DrawText(TextFormat("Time: %.2f", elapsedTime), 100, 100, 30, GREEN);

        } else { 
            DrawText("Trying...", 300, 150, 30, DARKGRAY);
            DrawText(TextFormat("Time: %.2f", currentTime), 100, 100, 30, RED);
        }
        
        DrawText(TextFormat("%c", current1Letter), 320, 200, 60, found ? GREEN : RED);
        DrawText(TextFormat("%c", current2Letter), 360, 200, 60, found ? GREEN : RED);
        DrawText(TextFormat("%c", current3Letter), 400, 200, 60, found ? GREEN : RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}