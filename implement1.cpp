#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>

using namespace std;

// 1. THE TOOLS (Functions)
char randomCharacter() {
    int c = GetRandomValue(32, 126);
    return (char)c; // returns random characters, turns numbers into ASCII character
}

// 2. THE BLUEPRINTS (Classes)
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

        void calculateFitness(const std::string& target) {
            int score = 0;
            for (int i = 0; i < genes.size(); i++) {
                if (genes[i] == target[i]) {
                    score ++;
                }
            }
            fitness = (float)score / (float)target.size();
        }

        int getFitness(){
            return fitness;
        }
};

// 3. THE GLOBALS AND SETUP

std::string target = "to be or not to be";
std::vector<DNA> population;
std::vector<std::string> matingPool;
void setup() {
    for (int i = 0; i < 100; i++) {
        population.push_back(DNA(18));
    }
}

int main() {
    InitWindow(1920, 1080, "First Run!");
    SetTargetFPS(60);
    // SetRandomSeed(42);

    DNA myMonkey(18);

    setup();

    while(!WindowShouldClose()) {

        for (DNA& phrase: population) {
            int n = (phrase.getFitness()) * 100;
            phrase.calculateFitness(target);
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        // DrawText(DrawFPS(), 100, 100, 30, GREEN);B
        DrawFPS(100, 100);
        DrawText("Raylib is running!", 100, 150, 20, RAYWHITE);
        DrawText(TextFormat("Population size: %d", population.size()), 100, 200, 20, RAYWHITE);
        
        for (int i = 0; i < 3; i++) {
            // 1. Get the genes from the current monkey in the population
            std::vector<char> currentGenes = population[i].getGenes();

            // 2. Convert the vector of characters into a C++ string
            std::string geneText(currentGenes.begin(), currentGenes.end());

            // 3. Calculate the Y position so they don't draw on top of each other
            // Monkey 0: Y = 200. Monkey 1: Y = 230. Monkey 2: Y = 260.
            int yPosition = 250 + (i * 30);

            // 4. Draw the text to the screen!
            // Note: Raylib requires '.c_str()' to read C++ strings properly
            DrawText(geneText.c_str(), 100, yPosition, 20, RAYWHITE);
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}