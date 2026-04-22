#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <time.h>
#include <raylib.h>
// #include <rtext>

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
float mutationRate = 0.01;
int populatoinSize = 125;
int generation = 0;
bool isFinished = false;
std::string target = "to be or not to be";
std::vector<DNA> population;


Color bgColor = { 40, 42, 54, 255 };
Color normalColor = { 98, 114, 164, 255 };
Color championColor = { 80, 250, 123, 255 };
Color headerColor = { 248, 248, 242, 255 };

// Step 1: Initialization
void setup() {
    for (int i = 0; i < populatoinSize; i++) {
        population.push_back(DNA(target.size()));
    }
}

int main() {
    srand(time(NULL));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "Monkey!");
    SetTargetFPS(60);
    // SetRandomSeed(42);

    double startTime = GetTime();
    double elapsedTime = 0;
    Font font = LoadFont("Consolas-Regular.ttf");

    // DNA myMonkey(target.size());
    setup();

    while(!WindowShouldClose()) {
        double currentTime = GetTime() - startTime;
        std::vector<DNA> matingPool;

        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // Step 2: Selection
        // Step 2a: Calculate the fitness score.
        // Calculate fitness of each monkey at each generation

        if (!isFinished) {


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
            
            for (DNA& phrase: population) {
                phrase.calculateFitness(target);
                elapsedTime = GetTime() - startTime;
                if (phrase.getFitness() >= 1.0f ){
                    isFinished = true;
                    elapsedTime = GetTime() - startTime;
                }
            }

            generation++;
        }

        // Finding the best phrase and index
        float bestPhraseScore = population[0].getFitness();
        DNA bestPhrase = population[0]; 
        int bestIndex = 0; 
        
        for (int i = 0; i < population.size(); i++) {
            if (population[i].getFitness() > bestPhraseScore) {
                bestPhraseScore = population[i].getFitness();
                bestPhrase = population[i];
                bestIndex = i; // Save the seat number!
            }
        }

        BeginDrawing();
        ClearBackground(bgColor);

        std::vector<char> bestGenes = bestPhrase.getGenes();
        std::string bestText(bestGenes.begin(), bestGenes.end());

        DrawFPS(10, 5);
        DrawTextEx(font, TextFormat("Population size: %d", populatoinSize), Vector2{20, 30}, 25, 2, headerColor);
        DrawTextEx(font, TextFormat("Generation: %d", generation), Vector2{20, 70}, 20, 2, headerColor);
        DrawTextEx(font, TextFormat("Time: %.2f", elapsedTime), Vector2{20, 110}, 20, 2, headerColor);

        DrawTextEx(font, TextFormat("Best Score: %.2f", bestPhraseScore), Vector2{250, 70}, 20, 2, headerColor);
        DrawTextEx(font, TextFormat("Mutation Rate: %.2f", mutationRate), Vector2{250, 110}, 20, 2, headerColor);
        DrawTextEx(font, TextFormat("Best Monkey: %s", bestText.c_str()), Vector2{900, 30}, 40, 2, headerColor);

        DrawLine(0, 150, 1920, 150, RAYWHITE);
        DrawLine(1, 1, 1920, 0, RAYWHITE);
        DrawLine(1, 1, 1, 1079, RAYWHITE);
        DrawLine(1919, 1, 1919, 1079, RAYWHITE);
        DrawLine(1, 1079, 1919, 1079, RAYWHITE);

        //DrawText(TextFormat("Best Monkey: %s", bestPhrase.c_str()), 500, 600, 20, RAYWHITE);
        

        for (int i = 0; i < population.size(); i++) {
            // 1. Get the genes from the current monkey in the population
            std::vector<char> currentGenes = population[i].getGenes();

            // 2. Convert the vector of characters into a C++ string
            std::string geneText(currentGenes.begin(), currentGenes.end());

            int xPosition = 130 + ((i / 25) * 350); 
            int yPosition = 200 + ((i % 25) * 30); 
            
            // Vector2 is required by DrawTextEx for position
            Vector2 position = { (float)xPosition, (float)yPosition };

            // 3. Set the color based on our bestIndex
            Color textColor = normalColor;
            if (i == bestIndex) {
                textColor = championColor;
            }

            DrawTextEx(font, geneText.c_str(), position, 24, 2, textColor);           
        }

        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}