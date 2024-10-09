#include <iostream>
#include <array>

#include "include/CompileTimeRandom.hpp"

int main()
{
    constexpr CCompileTimeRandom rng;

    std::cout << "Random Int: " << rng.GenerateInt() << "\n";
    std::cout << "Random Int [1, 10]: " << rng.GenerateIntInRange(1, 10) << "\n";
    std::cout << "Random Bool: " << (rng.GenerateBool() ? "true" : "false") << "\n";
    std::cout << "Random Float [0.0, 1.0): " << rng.GenerateFloat(0.0f, 1.0f) << "\n";

    // Generate a compile-time array of random integers
    constexpr auto randomArray = rng.GenerateIntArray<5>(1, 100);
    std::cout << "Random Array [1, 100]: ";
    for (const auto& val : randomArray)
    {
        std::cout << val << " ";
    }
    std::cout << "\n";

    // Shuffle the array at compile time
    constexpr auto shuffledArray = rng.ShuffleArray(randomArray);
    std::cout << "Shuffled Array: ";
    for (const auto& val : shuffledArray)
    {
        std::cout << val << " ";
    }
    std::cout << "\n";

    return 0;
}
