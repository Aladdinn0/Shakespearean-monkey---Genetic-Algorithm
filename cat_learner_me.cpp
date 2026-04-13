#include <raylib.h>

int main() {
    char letters[26] = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','z'
    };


    InitWindow(1920, 1080, "Hell Yeah!!");
    SetTargetFPS(60);
    SetRandomSeed(89);

    char firstLetter  = ' ';
    char secondLetter = ' ';
    char thirdLetter  = ' ';

    int frameCounter = 0;
    double startTime = GetTime();
    double elapsedTime = 0;

    bool foundC = false;
    bool foundA = false;
    bool foundT = false;

    while(!WindowShouldClose()){
        double currentTime = GetTime() - startTime;

        // LOGIC Part
        if (!(foundC && foundA && foundT)) {
            frameCounter++;

            if (frameCounter >= 2) {
                if (!foundC) {
                    firstLetter  = letters[GetRandomValue(0, 25)];
                    if (firstLetter == 'c') {
                        foundC = true;
                    }
                }

                if (!foundA) {
                    secondLetter  = letters[GetRandomValue(0, 25)];
                    if (secondLetter == 'a') {
                        foundA = true;
                    }
                }

                if (!foundT) {
                    thirdLetter  = letters[GetRandomValue(0, 25)];
                    if (thirdLetter == 't') {
                        foundT = true;
                    }
                }

                if (foundC && foundA && foundT) {
                    elapsedTime = GetTime() - currentTime;
                }

                frameCounter = 0;
            }
        }

        // Drawing Part
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        if (foundC && foundA && foundT) {
            DrawText("Cat found!!!", 800, 480, 40, GREEN);
        } else {
            DrawText("Searching...", 800, 480, 40, DARKGRAY);
            DrawText(TextFormat("Time: %.2f", currentTime), 10, 10 , 40, BLUE);
        }

        DrawText(TextFormat("%c", firstLetter), 800, 520, 35, foundC ? GREEN : RED);
        DrawText(TextFormat("%c", secondLetter), 840, 520, 35, foundA ? GREEN : RED);
        DrawText(TextFormat("%c", thirdLetter), 880, 520, 35, foundT ? GREEN : RED);
        DrawText(TextFormat("Time: %.2f", elapsedTime), 10, 10 , 40, GREEN);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}