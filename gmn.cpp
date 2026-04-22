#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <raylib.h>
#include <raymath.h> // NEW: Required for Vector2 distance and addition!

using namespace std;

// ==========================================
// GLOBALS & CONSTANTS
// ==========================================
int lifespan = 400;     // How many frames one generation lasts
int lifeCounter = 0;    // Current frame of the generation
int generation = 1;
float maxForce = 0.5f;  // How hard the thrusters can push
float mutationRate = 0.01f;

Vector2 target = { 960, 100 }; // Top middle of the screen

// ==========================================
// 1. THE TOOLS
// ==========================================
// Generates a random 2D vector (an arrow pointing in a random direction)
Vector2 randomVector() {
    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
    return { cos(angle) * maxForce, sin(angle) * maxForce };
}

// ==========================================
// 2. THE BLUEPRINTS
// ==========================================
class DNA {
public:
    std::vector<Vector2> genes;

    // Build random DNA for the whole lifespan
    DNA() {
        for (int i = 0; i < lifespan; i++) {
            genes.push_back(randomVector());
        }
    }

    DNA crossover(const DNA& partner) {
        DNA child;
        child.genes.clear(); // Clear default random genes
        int midpoint = GetRandomValue(0, lifespan);
        
        for (int i = 0; i < lifespan; i++) {
            if (i > midpoint) child.genes.push_back(genes[i]);
            else child.genes.push_back(partner.genes[i]);
        }
        return child;
    }

    void mutate() {
        for (int i = 0; i < lifespan; i++) {
            float compareRate = (float)rand() / RAND_MAX;
            if (compareRate < mutationRate) {
                genes[i] = randomVector();
            }
        }
    }
};

class Rocket {
public:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    DNA dna;
    float fitness = 0.0f;
    bool completed = false;
    bool crashed = false;

    // All rockets start at the bottom middle of the screen
    Rocket() {
        position = { 960, 1000 };
        velocity = { 0, 0 };
        acceleration = { 0, 0 };
    }

    void applyForce(Vector2 force) {
        acceleration.x += force.x;
        acceleration.y += force.y;
    }

    void update(int currentFrame) {
        // Check if we hit the target!
        float distToTarget = Vector2Distance(position, target);
        if (distToTarget < 25) {
            completed = true;
            position = target; // Snap to target
        }

        // Check if we hit the edge of the screen
        if (position.x < 0 || position.x > 1920 || position.y < 0 || position.y > 1080) {
            crashed = true;
        }

        // Only move if we haven't crashed and haven't finished
        if (!completed && !crashed) {
            applyForce(dna.genes[currentFrame]); // Fire the thruster for this specific frame
            
            velocity.x += acceleration.x;
            velocity.y += acceleration.y;
            position.x += velocity.x;
            position.y += velocity.y;
            
            acceleration = { 0, 0 }; // Reset force for the next frame
        }
    }

    void calculateFitness() {
        float distance = Vector2Distance(position, target);
        
        // Closer to target = higher fitness. We use 1 divided by distance.
        // We add 1.0f to prevent dividing by zero!
        fitness = 1.0f / (distance + 1.0f);

        // MASSIVE REWARD for reaching the target
        if (completed) fitness *= 10;
        
        // PUNISHMENT for crashing into the walls
        if (crashed) fitness /= 10;
        
        // Square the fitness to make the good ones exponentially better
        // (Just like multiplying the pool tickets in the monkey code)
        fitness *= fitness; 
    }

    void draw() {
        // Draw the rocket as a small triangle pointing in the direction of velocity
        float angle = atan2(velocity.y, velocity.x) * RAD2DEG;
        
        Color rColor = { 150, 150, 150, 150 }; // Ghostly white/gray
        if (completed) rColor = GREEN;
        if (crashed) rColor = RED;

        // Note: DrawPoly draws a regular polygon. 3 sides = triangle!
        DrawPoly(position, 3, 10, angle, rColor);
    }
};

// ==========================================
// 3. MAIN GAME LOOP
// ==========================================
int main() {
    srand(time(NULL));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1920, 1080, "Smart Rockets!");
    SetTargetFPS(6000);

    // Create the population
    std::vector<Rocket> population;
    for (int i = 0; i < 200; i++) {
        population.push_back(Rocket());
    }

    while (!WindowShouldClose()) {
        
        // ----------------------------------------------------
        // LOGIC
        // ----------------------------------------------------
        
        if (lifeCounter < lifespan) {
            // GENERATION IS RUNNING: Update all rockets
            for (Rocket& r : population) {
                r.update(lifeCounter);
            }
            lifeCounter++;
        } else {
            // GENERATION FINISHED: Time to evolve!
            
            // 1. Calculate fitness
            float maxFitness = 0.0f;
            for (Rocket& r : population) {
                r.calculateFitness();
                if (r.fitness > maxFitness) maxFitness = r.fitness;
            }

            // Normalize fitness (scale them between 0.0 and 1.0)
            // This makes building the mating pool much easier
            for (Rocket& r : population) {
                r.fitness /= maxFitness;
            }

            // 2. Build mating pool
            std::vector<Rocket> matingPool;
            for (Rocket& r : population) {
                int n = std::floor(r.fitness * 100);
                if (n == 0) n = 1; // Pity ticket
                for (int i = 0; i < n; i++) {
                    matingPool.push_back(r);
                }
            }

            // 3. Reproduction
            std::vector<Rocket> newPopulation;
            for (int i = 0; i < population.size(); i++) {
                int indexA = GetRandomValue(0, matingPool.size() - 1);
                int indexB = GetRandomValue(0, matingPool.size() - 1);
                
                DNA childDNA = matingPool[indexA].dna.crossover(matingPool[indexB].dna);
                childDNA.mutate();
                
                // Create a brand new rocket at the starting line with the child DNA
                Rocket child;
                child.dna = childDNA;
                newPopulation.push_back(child);
            }

            population = newPopulation;
            lifeCounter = 0; // Reset the clock!
            generation++;
        }

        // ----------------------------------------------------
        // RENDERING
        // ----------------------------------------------------
        BeginDrawing();
        ClearBackground({ 15, 15, 25, 255 }); // Dark Cyberpunk background

        // Draw the target
        DrawCircleV(target, 25, { 80, 250, 123, 255 }); // Bright Green Target

        // Draw all rockets
        for (Rocket& r : population) {
            r.draw();
        }

        // Dashboard
        DrawText(TextFormat("Generation: %d", generation), 20, 20, 20, RAYWHITE);
        DrawText(TextFormat("Frames left: %d", lifespan - lifeCounter), 20, 50, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


// #include <iostream>
// #include <vector>
// #include <string>
// #include <cmath>
// #include <time.h>
// #include <raylib.h>

// using namespace std;

// // 1. THE TOOLS (Functions)
// char randomCharacter() {
//     int c = GetRandomValue(32, 126);
//     return (char)c; // returns random characters, turns numbers into ASCII character
// }

// // 2. THE BLUEPRINTS (Classes)
// class DNA {
//     private:
//         std::vector<char> genes;
//         float fitness = 0.0;
//     public:
//         DNA(int length){
//             for (int i = 0; i < length; i++) {
//                 genes.push_back(randomCharacter());
//             }
//         }

//         std::vector<char> getGenes(){
//             return genes;
//         }

//         void calculateFitness(const std::string& target) {
//             int score = 0;
//             for (int i = 0; i < genes.size(); i++) {
//                 if (genes[i] == target[i]) {
//                     score ++;
//                 }
//             }
//             fitness = (float)score / (float)target.size();
//         }

//         float getFitness(){
//             return fitness;
//         }

//         void mutate(float mutationRate) {
//             for (int i = 0; i < genes.size(); i++) {
//                 float compareRate = (float)rand() / RAND_MAX;
//                 if (compareRate < mutationRate) {
//                     genes[i] = randomCharacter();
//                 }
//             }
//         }        
        
//         DNA crossover(const DNA& partner){
//             DNA child(genes.size());

//             int midpoint = GetRandomValue(0, genes.size());
//             for (int i = 0; i < genes.size(); i++) {
//                 if (i < midpoint) {
//                     child.genes[i] = genes[i];
//                 } else {
//                     child.genes[i] = partner.genes[i];
//                 }
//             }
//             return child;
//     }           
// };

// // 3. THE GLOBALS AND SETUP
// float mutationRate = 0.01;
// int populatoinSize = 125;
// int generation = 0;
// bool isFinished = false;
// std::string target = "to be or not to be";
// std::vector<DNA> population;

// // NEW: Array to track the chart history
// std::vector<float> averageFitnessHistory; 

// Color bgColor = { 40, 42, 54, 255 };
// Color normalColor = { 98, 114, 164, 255 };
// Color championColor = { 80, 250, 123, 255 };
// Color headerColor = { 248, 248, 242, 255 };

// // Step 1: Initialization
// void setup() {
//     for (int i = 0; i < populatoinSize; i++) {
//         population.push_back(DNA(target.size()));
//     }
// }

// int main() {
//     srand(time(NULL));
//     SetConfigFlags(FLAG_WINDOW_RESIZABLE);
//     InitWindow(1920, 1080, "Monkey Evolution!");
//     SetTargetFPS(60);

//     Font font = LoadFont("Consolas-Regular.ttf");

//     setup();

//     while(!WindowShouldClose()) {
//         std::vector<DNA> matingPool;

//         if (IsKeyPressed(KEY_F11)) {
//             ToggleFullscreen();
//         }

//         // Step 2: Selection
//         if (!isFinished) {

//             // Step 2b: Build the mating pool.
//             for (DNA& phrase: population) {
//                 int n = std::floor((phrase.getFitness()) * 100);
//                 for (int i = 0; i < n; i++) {                      
//                     matingPool.push_back(phrase);
//                 }
//             }

//             // Step 3: Reproduction
//             if (matingPool.size() > 0) {
//                 for (int i = 0; i < population.size(); i++){
//                     int indexA = GetRandomValue(0, matingPool.size() - 1);
//                     int indexB = GetRandomValue(0, matingPool.size() - 1);
//                     DNA parentA = matingPool[indexA];
//                     DNA parentB = matingPool[indexB];

//                     DNA child  = parentA.crossover(parentB);
//                     child.mutate(mutationRate);
//                     population[i] = child;
//                 }
//             }
            
//             for (DNA& phrase: population) {
//                 phrase.calculateFitness(target);
//                 if (phrase.getFitness() >= 1.0f ){
//                     isFinished = true;
//                 }
//             }

//             generation++;
//         }


//         // Finding the best phrase and calculating the AVERAGE
//         float bestPhraseScore = population[0].getFitness();
//         DNA bestPhrase = population[0]; 
//         int bestIndex = 0; 
//         float totalFitness = 0.0f; // Track total to find the average
        
//         for (int i = 0; i < population.size(); i++) {
//             totalFitness += population[i].getFitness(); // Add everyone's score
            
//             if (population[i].getFitness() > bestPhraseScore) {
//                 bestPhraseScore = population[i].getFitness();
//                 bestPhrase = population[i];
//                 bestIndex = i; // Save the seat number!
//             }
//         }
        
//         // Save the average for the chart (Freeze it when the game is won)
//         if (!isFinished) {
//             float currentAvg = totalFitness / population.size();
//             averageFitnessHistory.push_back(currentAvg);
//         }

//         BeginDrawing();
//         ClearBackground(bgColor);

//         std::vector<char> bestGenes = bestPhrase.getGenes();
//         std::string bestText(bestGenes.begin(), bestGenes.end());

//         DrawFPS(10, 5);
//         DrawTextEx(font, TextFormat("Population size: %d", populatoinSize), Vector2{10, 30}, 25, 2, headerColor);
//         DrawTextEx(font, TextFormat("Generation: %d", generation), Vector2{10, 60}, 20, 2, headerColor);
//         DrawTextEx(font, TextFormat("Best Score: %.2f", bestPhraseScore), Vector2{400, 30}, 20, 2, headerColor);
//         DrawTextEx(font, TextFormat("Mutation Rate: %.2f", mutationRate), Vector2{400, 60}, 20, 2, headerColor);
//         DrawTextEx(font, TextFormat("Best Monkey: %s", bestText.c_str()), Vector2{900, 30}, 40, 2, headerColor);

//         // --- NEW: DRAW AVERAGE FITNESS CHART ---
//         int screenWidth = GetScreenWidth();
//         float chartWidth = 350.0f;
//         float chartHeight = 100.0f; // Squished slightly to fit your header
//         float chartX = screenWidth - chartWidth - 20.0f; 
//         float chartY = 35.0f;

//         DrawRectangle(chartX, chartY, chartWidth, chartHeight, { 25, 25, 35, 255 }); 
//         DrawRectangleLines(chartX, chartY, chartWidth, chartHeight, headerColor);
//         DrawTextEx(font, "Avg Fitness", Vector2{chartX, chartY - 25}, 20, 2, headerColor);

//         if (averageFitnessHistory.size() > 1) {
//             float xStep = chartWidth / (averageFitnessHistory.size() - 1);
//             for (int i = 0; i < averageFitnessHistory.size() - 1; i++) {
//                 // Since Monkey fitness is exactly 0.0 to 1.0, math is simple!
//                 float y1 = chartY + chartHeight - (averageFitnessHistory[i] * chartHeight);
//                 float y2 = chartY + chartHeight - (averageFitnessHistory[i + 1] * chartHeight);
                
//                 float x1 = chartX + (i * xStep);
//                 float x2 = chartX + ((i + 1) * xStep);

//                 DrawLineEx({x1, y1}, {x2, y2}, 2.0f, championColor); 
//             }
//         }
//         // ---------------------------------------

//         // Boundaries
//         DrawLine(0, 150, 1920, 150, RAYWHITE);
//         DrawLine(1, 1, 1920, 0, RAYWHITE);
//         DrawLine(1, 1, 1, 1079, RAYWHITE);
//         DrawLine(1919, 1, 1919, 1079, RAYWHITE);
//         DrawLine(1, 1079, 1919, 1079, RAYWHITE);

//         // Draw the Monkeys
//         for (int i = 0; i < population.size(); i++) {
//             std::vector<char> currentGenes = population[i].getGenes();
//             std::string geneText(currentGenes.begin(), currentGenes.end());

//             int xPosition = 130 + ((i / 25) * 350); 
//             int yPosition = 200 + ((i % 25) * 30); 
            
//             Vector2 position = { (float)xPosition, (float)yPosition };

//             Color textColor = normalColor;
//             if (i == bestIndex) {
//                 textColor = championColor;
//             }

//             DrawTextEx(font, geneText.c_str(), position, 24, 2, textColor);           
//         }

//         EndDrawing();
//     }
//     UnloadFont(font);
//     CloseWindow();
//     return 0;
// }