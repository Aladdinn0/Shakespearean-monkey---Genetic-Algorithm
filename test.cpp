#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <raylib.h>

/*
class DNA {
    private:
        std::vector<char> genes;
        int fitness = 0;
    public:
        DNA(int length){
            for (int i = 0; i < length; i++) {
                genes.push_back(randomCharacter());
            }
        }

        std::vector<char> getGenes(){
            return genes;
        }

        void calculateFitness(std::string target) {
            int score = 0;
            for (int i = 0; i < genes.size(); i++) {
                if (genes[i] == target[i]) {
                    score ++;
                }
            }
            fitness = score / target.size();
        }
};
*/
// 3. THE GLOBALS AND SETUP

std::string target = "to be or not to be";
// std::vector<DNA> population;
int main() {
    srand(time(NULL));
    InitWindow(1000, 1000, "What hte Heck?");
    // float a = GetRandomValue(0.0,1.0);
    float a = rand();
    float b = RAND_MAX;
    std::vector<float> values; 
    while(!WindowShouldClose()) {

       if (IsKeyPressed(KEY_SPACE)) {
            values.clear();
            for (int i = 0; i < 30; i++) {
                values.push_back((float)rand() / RAND_MAX);
            }
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        for (int i = 0; i < values.size(); i++) {
            float c = values[i];

            if (c < 0.1f) {
                DrawText(TextFormat("%.5f True", c), 100, i * 30 + 50, 20, GREEN);
            } else {
                DrawText(TextFormat("%.5f False", c), 100, i * 30 + 50, 20, RED);
            }
        }

        EndDrawing();
    }
    CloseWindow();


    return 0;
}
