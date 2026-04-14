#include <iostream>
#include <vector>
#include <string>

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

    std::string a = "salam";

    std::cout << a[2] << "\n";
    std::cout << "Cavab: " << 4.0/5.0 << "\n";

    return 0;
}