#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <time.h>
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
        float fitness = 0.0;
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

        float getFitness(){
            return fitness;
        }

        void mutate(float mutationRate) {
            for (int i = 0; i < genes.size(); i++) {
                float compareRate = (float)rand() / RAND_MAX;
                if (compareRate < mutationRate) {
                    genes[i] = randomCharacter();
                }
            }
        }        
        
        DNA crossover(const DNA& partner){
            DNA child(genes.size());

            int midpoint = GetRandomValue(0, genes.size());
            for (int i = 0; i < genes.size(); i++) {
                if (i < midpoint) {
                    child.genes[i] = genes[i];
                } else {
                    child.genes[i] = partner.genes[i];
                }
            }
            return child;
    }           
};

// 3. THE GLOBALS AND SETUP
// FONT font = LoadFont("resources/myfont.ttf");
float mutationRate = 0.01;
int populatoinSize = 15000;
int generation = 0;
bool isFinished = false;
std::string target = "to be or not to be";
std::vector<DNA> population;

// Step 1: Initialization
void setup() {
    for (int i = 0; i < 100; i++) {
        population.push_back(DNA(target.size()));
    }
}

int main() {
    srand(time(NULL));
    InitWindow(1920, 1080, "First Run!");
    SetTargetFPS(60);
    // SetRandomSeed(42);

    DNA myMonkey(target.size());
    setup();

    while(!WindowShouldClose()) {
        std::vector<DNA> matingPool;

        // Step 2: Selection
        // Step 2a: Calculate the firness score.
        // Calculate fitness of each monkey at each generation

        if (!isFinished) {
            for (DNA& phrase: population) {
                phrase.calculateFitness(target);
                if (phrase.getFitness() >= 1.0f ){
                    isFinished = true;
                }
            }

            // Step 2b: Build the mating pool.
            for (DNA& phrase: population) {
                int n = std::floor((phrase.getFitness()) * 100);
                for (int i = 0; i < n; i++) {                      // fitness score sayi qeder phrase mating poola elave olunacaq
                    matingPool.push_back(phrase);
                }
            }

            // Step 3: Reproduction
            if (matingPool.size() > 0) {
                for (int i = 0; i < population.size(); i++){
                    int indexA = GetRandomValue(0, matingPool.size() - 1);
                    int indexB = GetRandomValue(0, matingPool.size() - 1);
                    DNA parentA = matingPool[indexA];
                    DNA parentB = matingPool[indexB];

                    DNA child  = parentA.crossover(parentB);
                    child.mutate(mutationRate);
                    population[i] = child;
                }
            }
            generation++;
        }


        // Finding the best match 
        float bestPhraseScore = population[0].getFitness();
        DNA bestPhrase = population[0];
        for (DNA& phrase: population) {
            if (phrase.getFitness() > bestPhraseScore) {
                bestPhraseScore = phrase.getFitness();
                bestPhrase = phrase;
            }
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        std::vector<char> bestGenes = bestPhrase.getGenes();
        std::string bestText(bestGenes.begin(), bestGenes.end());

        // DrawText(DrawFPS(), 100, 100, 30, GREEN);B
        DrawFPS(100, 100);
        DrawText("Raylib is running!", 100, 150, 20, RAYWHITE);
        DrawText(TextFormat("Population size: %d", population.size()), 100, 200, 20, RAYWHITE);
        DrawText(TextFormat("Generation: %d", generation), 500, 500, 20, RAYWHITE);
        // DrawText(TextFormat("Phrase: %d", generation), 500, 500, 20, RAYWHITE);
        DrawText(TextFormat("Best Score: %f", bestPhraseScore), 500, 550, 20, RAYWHITE);
        DrawText(TextFormat("Best Monkey: %s", bestText.c_str()), 500, 600, 20, RAYWHITE);
        //DrawText(TextFormat("Best Monkey: %s", bestPhrase.c_str()), 500, 600, 20, RAYWHITE);
        DrawText(TextFormat("Mutation Rate: %.2f", mutationRate), 500, 650, 20, RAYWHITE);
        

        for (int i = 0; i < population.size(); i++) {
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