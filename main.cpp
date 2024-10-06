#include <iostream>

#include "include/CompileTimeRandom.hpp"

int main()
{
    // Example 1: Use default constructor with __TIME__ as the seed
    constexpr CCompileTimeRandom randGen;

    // Generate a random number at compile time
    constexpr unsigned int dwRandomNumber = randGen.GenerateMoreRandom();
    std::cout << "Random number at compile time (using __TIME__): " << dwRandomNumber << std::endl;

    // Example 2: Use custom seed
    constexpr CCompileTimeRandom customRandGen(98765);
    constexpr unsigned int customRandomNumber = customRandGen.GenerateMoreRandom();
    std::cout << "Random number with custom seed: " << customRandomNumber << std::endl;

    return 0;
}